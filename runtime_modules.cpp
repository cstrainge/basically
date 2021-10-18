
#include "basically.h"


namespace basically::runtime::modules
{


    namespace
    {


        void add_number_type(ModulePtr module,
                             std::string const& name,
                             typing::SignedFlag is_signed,
                             typing::FloatingPointFlag is_floating_point,
                             size_t size)
        {
            auto extra = std::make_shared<typing::NumberInfo>(is_signed,
                                                              is_floating_point,
                                                              size);

            module->insert(std::make_shared<typing::TypeInfo>(name,
                                                              extra,
                                                              typing::Visibility::Public));
        }


        ModulePtr create_builtins()
        {
            const auto is_signed = typing::SignedFlag::IsSigned;
            const auto is_unsigned = typing::SignedFlag::IsUnsigned;
            const auto is_integer = typing::FloatingPointFlag::IsInteger;
            const auto is_float = typing::FloatingPointFlag::IsFloatingPoint;

            auto builtins = std::make_shared<Module>();

            add_number_type(builtins, "i8", is_signed, is_integer, 1);
            add_number_type(builtins, "u8", is_unsigned, is_integer, 1);

            add_number_type(builtins, "i16", is_signed, is_integer, 2);
            add_number_type(builtins, "u16", is_unsigned, is_integer, 2);

            add_number_type(builtins, "i32", is_signed, is_integer, 4);
            add_number_type(builtins, "u32", is_unsigned, is_integer, 4);

            add_number_type(builtins, "i64", is_signed, is_integer, 8);
            add_number_type(builtins, "u64", is_unsigned, is_integer, 8);

            add_number_type(builtins, "f32", is_signed, is_float, 4);
            add_number_type(builtins, "f64", is_signed, is_float, 8);

            return builtins;
        }


        template <typename DataType>
        auto bind(Module* module, DataType handler)
        {
            return std::bind(handler, module, std::placeholders::_1);
        }


    }


    ModulePtr& get_builtins_module()
    {
        static ModulePtr builtins = create_builtins();
        return builtins;
    }


    Module::Module(std::string const& new_name,
                   std::fs::path const& new_base_path,
                   ast::StatementList const& new_code,
                   Loader& loader)
    : name(new_name),
      base_path(new_base_path)
    {
        process_passs_1(new_code, loader);
        process_passs_2();
    }


    int Module::execute()
    {
        return 0;
    }


    void Module::process_passs_1(ast::StatementList const& ast, Loader& loader)
    {
        auto load_handler = [&](auto statement)
            {
                load_submodule(statement, loader);
            };

        auto statement_handlers = ast::StatementHandlers
            {
                .sub_declaration_statement       = bind(this, &Module::add_sub),
                .function_declaration_statement  = bind(this, &Module::add_function),
                .load_statement                  = load_handler,
                .structure_declaration_statement = bind(this, &Module::add_structure),
                .variable_declaration_statement  = bind(this, &Module::add_variable),

                .default_handler =
                    [&](auto statement)
                    {
                        startup.push_back(statement);
                    }
            };

        for (auto const& statement : ast)
        {
            std::visit(statement_handlers, statement);
        }
    }


    void Module::process_passs_2()
    {
    }


    void Module::load_submodule(ast::LoadStatementPtr const& statement, Loader& loader)
    {
    }


    void Module::add_sub(ast::SubDeclarationStatementPtr const& statement)
    {
    }


    void Module::add_function(ast::FunctionDeclarationStatementPtr const& statement)
    {
    }


    void Module::add_structure(ast::StructureDeclarationStatementPtr const& statemnent)
    {
    }


    void Module::add_variable(ast::VariableDeclarationStatementPtr const& statement)
    {
    }


    Loader::Loader()
    {
        loaded_modules.insert({ "builtins", get_builtins_module() });
    }


    void Loader::set_system_path(std::fs::path const& path)
    {
        std::cout << "Set system path " << path << "." << std::endl;
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
            std::cout << "Returning cached module: " << name << "." << std::endl;
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
        auto name_without_extension = without_extension(name);

        std::cout << "Loading module " << name_without_extension
                  << " from " << module_path << "."
                  << std::endl;

        auto new_module = std::make_shared<Module>(name_without_extension, module_path, ast, *this);

        loaded_modules.insert({ without_extension(name), new_module });

        return new_module;
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
