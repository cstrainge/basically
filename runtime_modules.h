
#pragma once


namespace basically::runtime::modules
{


    class Module;
    class Loader;

    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


    ModulePtr& get_builtins_module();


    class Module
    {
        private:
            std::string name;
            std::fs::path base_path;

            ModuleMap loaded_modules;

            typing::TypeInfoMap types;
            typing::SubInfoMap subs;
            typing::FunctionInfoMap functions;

            variables::ScopePtr variable_scope;

            ast::StatementList startup_ast;

            jitting::Jit jitter;
            std::function<int()> init_function = []() { return EXIT_FAILURE; };

        public:
            Module() = default;
            Module(std::string const& new_name,
                   std::fs::path const& new_base_path,
                   ast::StatementList const& new_ast,
                   Loader& loader);
            Module(Module const& module) = delete;
            Module(Module&& module) = delete;
            ~Module() = default;

        public:
            Module& operator =(Module const&& module) = delete;
            Module& operator =(Module& module) = delete;

        public:
            int execute();

        private:
            void process_passs_1(ast::StatementList const& ast, Loader& loader);
            void process_passs_2();
            void process_passs_3();

            void load_submodule(ast::LoadStatementPtr const& statement, Loader& loader);

            void add_sub(ast::SubDeclarationStatementPtr const& statement);
            void add_function(ast::FunctionDeclarationStatementPtr const& statement);
            void add_structure(ast::StructureDeclarationStatementPtr const& statemnent);
            void add_variable(ast::VariableDeclarationStatementPtr const& statement);

        public:
            void insert(typing::TypeInfoPtr&& item);

        private:
            template <typename FunctionType, typename... ExtraParams>
            auto bind(FunctionType function, ExtraParams... params)
            {
                return std::bind(function, this, std::placeholders::_1, params...);
            }

        private:
            template <typename ObjectType, typename StatementType>
            void insert_object(
                           std::unordered_map<std::string, std::shared_ptr<ObjectType>>& collection,
                           std::string const& type_name,
                           StatementType const& statement) const;

            template <typename CollectionType>
            void ensure_unique(CollectionType const& collection,
                               source::Location const& location,
                               std::string const& type_name,
                               std::string const& name) const;

        private:
            [[noreturn]]
            void duplicate_definition(source::Location const& location,
                                      std::string const& type_name,
                                      std::string const& name) const;
            [[noreturn]]
            void runtime_error(source::Location const& location, std::string const&& message) const;
    };


    class Loader
    {
        public:
            struct PathManager
            {
                Loader& loader;

                PathManager(Loader* new_loader, std::fs::path& new_path)
                : loader(*new_loader)
                {
                    assert(new_loader != nullptr);
                    loader.push_working_path(new_path);
                }

                ~PathManager()
                {
                    assert(!loader.working_path.empty());
                    loader.pop_working_path();
                }
            };

        private:
            std::fs::path system_path;
            std::list<std::fs::path> working_path;

            ModuleMap loaded_modules;

        public:
            Loader();
            ~Loader() = default;

        public:
            void set_system_path(std::fs::path const& path);

            void push_working_path(std::fs::path const& path);
            void pop_working_path();

        public:
            ModulePtr get_script(std::fs::path const& script_path);
            ModulePtr get_module(std::fs::path const& name);

        private:
            ModulePtr find_loaded_module(std::fs::path const& name);
            OptionalPath find_module_path(std::fs::path const& name) const;

            std::fs::path with_extension(std::fs::path name) const;
            std::fs::path without_extension(std::fs::path name) const;

            bool is_readable(std::fs::path const& path) const noexcept;
    };


}
