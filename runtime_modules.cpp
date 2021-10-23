
#include "basically.h"


namespace basically::runtime::modules
{


    namespace
    {


        ModulePtr create_builtins()
        {
            auto builtins = std::make_shared<Module>();

            auto add_number_type = [&](std::string const& name,
                                       typing::SignedFlag is_signed,
                                       typing::FloatingPointFlag is_floating_point,
                                       size_t size)
                {
                    auto extra = std::make_shared<typing::NumberInfo>(is_signed,
                                                                      is_floating_point,
                                                                      size);

                    builtins->insert(std::make_shared<typing::TypeInfo>(
                                                                       name,
                                                                       extra,
                                                                       typing::Visibility::Public));
                };

            const auto is_signed = typing::SignedFlag::IsSigned;
            const auto is_unsigned = typing::SignedFlag::IsUnsigned;
            const auto is_integer = typing::FloatingPointFlag::IsInteger;
            const auto is_float = typing::FloatingPointFlag::IsFloatingPoint;

            add_number_type("i8", is_signed, is_integer, 1);
            add_number_type("u8", is_unsigned, is_integer, 1);

            add_number_type("i16", is_signed, is_integer, 2);
            add_number_type("u16", is_unsigned, is_integer, 2);

            add_number_type("i32", is_signed, is_integer, 4);
            add_number_type("u32", is_unsigned, is_integer, 4);

            add_number_type("i64", is_signed, is_integer, 8);
            add_number_type("u64", is_unsigned, is_integer, 8);

            add_number_type("f32", is_signed, is_float, 4);
            add_number_type("f64", is_signed, is_float, 8);

            return builtins;
        }


    }


    ModulePtr& get_builtins_module()
    {
        static ModulePtr builtins = create_builtins();
        return builtins;
    }


    Module::Module(std::string const& new_name,
                   std::fs::path const& new_base_path,
                   ast::StatementList const& new_ast,
                   Loader& loader)
    : name(new_name),
      base_path(new_base_path),
      variable_scope(std::make_shared<variables::Scope>())
    {
        // Construct types, import code.
        process_passs_1(new_ast, loader);

        // Reslolve all references to the types, subs, and functions of this module and it's
        // imports.
        process_passs_2();

        // If we get this far, the module is technically correct, so compile what we have to machine
        // code and make sure it's ready to run.
        process_passs_3();
    }


    int Module::execute()
    {
        return init_function();
    }


    void Module::process_passs_1(ast::StatementList const& ast, Loader& loader)
    {
        auto statement_handlers = ast::StatementHandlers
            {
                .function_declaration_statement  = bind(&Module::add_function),
                .load_statement                  = bind(&Module::load_submodule, loader),
                .structure_declaration_statement = bind(&Module::add_structure),
                .sub_declaration_statement       = bind(&Module::add_sub),
                .variable_declaration_statement  = bind(&Module::add_variable),

                .default_handler =
                    [&](auto statement)
                    {
                        std::cout << "Add statement to " << name << " initilization."
                                  << std::endl;

                        startup_ast.push_back(statement);
                    }
            };

        create_variable("result", "i8", lexing::Type::LiteralInt, "0");
        create_variable("name", "string", lexing::Type::LiteralString, name);
        create_variable("base_path", "string", lexing::Type::LiteralString, base_path.string());

        for (auto const& statement : ast)
        {
            std::visit(statement_handlers, statement);
        }
    }


    void Module::process_passs_2()
    {
        // Resolve type/function references...
    }


    void Module::process_passs_3()
    {
        // Jit compile all the code...
    }


    void Module::load_submodule(ast::LoadStatementPtr const& statement, Loader& loader)
    {
        assert(statement->module_name.type == lexing::Type::Identifier);

        auto name = statement->module_name.text;

        if (loaded_modules.find(name) != loaded_modules.end())
        {
            runtime_error(statement->location, "Requested module, " + name + ", already loaded.");
        }

        auto module = loader.get_module(name);

        if (!module)
        {
            runtime_error(statement->location, "Could not load the module, " + name + ".");
        }

        if (statement->alias.type != lexing::Type::None)
        {
            assert(statement->alias.type == lexing::Type::Identifier);
            name = statement->alias.text;
        }

        loaded_modules.insert({ name, module });
    }


    void Module::create_variable(std::string const& name,
                                 std::string const& type_name,
                                 lexing::Type literal_type,
                                 std::string const& literal_value)
    {
        auto id_token = [](auto const& name) -> lexing::Token
            {
                return lexing::Token { .type = lexing::Type::Identifier, .text = name };
            };

        auto literal_expression = [](auto type, auto value) -> ast::Expression
            {
                auto value_token = lexing::Token { .type = type, .text = value };
                return std::make_shared<ast::LiteralExpression>(value_token);
            };

        auto declaration = [](auto const& name,
                              auto const& type,
                              auto const& init) -> ast::VariableDeclarationStatementPtr
            {
                return std::make_shared<ast::VariableDeclarationStatement>(source::Location {},
                                                                           name,
                                                                           type,
                                                                           init);
            };

        auto var = declaration(id_token(name),
                               id_token(type_name),
                               literal_expression(literal_type, literal_value));

        add_variable(var);
    }


    void Module::add_sub(ast::SubDeclarationStatementPtr const& statement)
    {
        insert_object(subs, "sub", statement);
    }


    void Module::add_function(ast::FunctionDeclarationStatementPtr const& statement)
    {
        insert_object(functions, "function", statement);
    }


    void Module::add_structure(ast::StructureDeclarationStatementPtr const& statement)
    {
        insert_object(types, "structure", statement);
    }


    void Module::add_variable(ast::VariableDeclarationStatementPtr const& statement)
    {
        // Create a variable declaration in the module's global scope.
        insert_object(variable_scope->variables, "variable", statement);

        // If the variable declaration has an initializer expression, add the declaration to the
        // start-up code so that it can be compiled in.
        if (statement->initializer)
        {
            startup_ast.push_back(statement);
        }
    }


    template <typename ObjectType, typename StatementType>
    void Module::insert_object(
                           std::unordered_map<std::string, std::shared_ptr<ObjectType>>& collection,
                           std::string const& object_type_name,
                           StatementType const& statement) const
    {
        assert(statement->name.type == lexing::Type::Identifier);

        auto object_name = statement->name.text;
        auto new_object = std::make_shared<ObjectType>(statement);

        std::cout << "Add " << object_type_name << " " << name << "." << object_name << "."
                  << std::endl;

        ensure_unique(collection, statement->location, object_type_name, object_name);
        collection.insert({ object_name, new_object });
    }


    template <typename CollectionType>
    void Module::ensure_unique(CollectionType const& collection,
                               source::Location const& location,
                               std::string const& object_type_name,
                               std::string const& name) const
    {
        if (collection.find(name) != collection.end())
        {
            duplicate_definition(location, object_type_name, name);
        }
    }


    void Module::duplicate_definition(source::Location const& location,
                                      std::string const& type_name,
                                      std::string const& name) const
    {
        runtime_error(location, "Duplicate definition for " + type_name + ", " + name + ".");
    }


    void Module::runtime_error(source::Location const& location, std::string const&& message) const
    {
        std::stringstream stream;

        stream << location << message;
        throw std::runtime_error(stream.str());
    }


    Loader::Loader()
    {
        loaded_modules.insert({ "builtins", get_builtins_module() });
    }


    void Loader::set_system_path(std::fs::path const& path)
    {
        system_path = path;
    }


    void Loader::push_working_path(std::fs::path const& path)
    {
        auto status = std::fs::status(path);

        if (!std::fs::exists(status))
        {
            throw std::runtime_error("Missing working directory, " + path.string() + ".");
        }

        if (!std::fs::is_directory(status))
        {
            throw std::runtime_error("Tried to set the working path to a non-directory," +
                                     path.string() + ".");
        }

        if (!is_readable(path))
        {
            throw std::runtime_error("Can not read from working path, " + path.string() + ".");
        }

        working_path.push_front(path);
    }


    void Loader::pop_working_path()
    {
        assert(!working_path.empty());
        working_path.pop_front();
    }


    ModulePtr Loader::get_script(std::fs::path const& script_path)
    {
        assert(script_path.has_filename());

        auto path_components = [&]() -> std::pair<std::fs::path, std::fs::path>
            {
                auto base_path = script_path;

                if (base_path.is_relative())
                {
                    base_path = std::fs::absolute(base_path);
                }

                base_path = base_path.lexically_normal();
                base_path.remove_filename();

                return { base_path, script_path.filename() };
            };

        auto [ base_path, file_name ] = path_components();
        PathManager path_push(this, base_path);

        return get_module(file_name);
    }


    ModulePtr Loader::get_module(std::fs::path const& name)
    {
        assert(!name.empty());
        assert(!system_path.empty());
        assert(!working_path.empty());

        if (auto found_module = find_loaded_module(name); found_module)
        {
            return found_module;
        }

        auto found_path = find_module_path(name);

        if (!found_path)
        {
            return nullptr;
        }

        auto module_path = found_path.value();

        auto source_buffer = source::Buffer(module_path);
        auto token_buffer = lexing::Buffer(source_buffer);

        auto ast = parsing::parse_to_ast(token_buffer);
        std::cout << std::endl << ast << std::endl;

        auto name_without_extension = without_extension(name);
        auto new_module = std::make_shared<Module>(name_without_extension,
                                                   module_path,
                                                   ast,
                                                   *this);

        loaded_modules.insert({ name_without_extension, new_module });

        return get_builtins_module();
    }


    ModulePtr Loader::find_loaded_module(std::fs::path const& name)
    {
        if (auto found = loaded_modules.find(without_extension(name));
            found != loaded_modules.end())
        {
            return found->second;
        }

        return nullptr;
    }


    OptionalPath Loader::find_module_path(std::fs::path const& name) const
    {
        assert(!working_path.empty());

        auto& base_path = working_path.front();
        auto module_path = base_path / with_extension(name);

        if (std::fs::exists(module_path))
        {
            return module_path.lexically_normal();
        }

        module_path = base_path / without_extension(name);

        if (!std::fs::exists(module_path))
        {
            throw std::runtime_error("Could not find module \"" +
                                     without_extension(name).string() +
                                     "\".");
        }

        return module_path.lexically_normal();
    }


    std::fs::path Loader::with_extension(std::fs::path name) const
    {
        if (!name.has_extension())
        {
            name.replace_extension(".bas");
        }

        return name;
    }


    std::fs::path Loader::without_extension(std::fs::path name) const
    {
        if (name.has_extension())
        {
            name.replace_extension("");
        }

        return name;
    }


    bool Loader::is_readable(std::fs::path const& path) const noexcept
    {
        return access(path.c_str(), R_OK) != -1;
    }


    void Module::insert(typing::TypeInfoPtr&& item)
    {
        types.emplace(item->name, std::move(item));
    }


}
