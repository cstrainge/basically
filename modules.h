
#pragma once


namespace basically::modules
{


    class Module;
    class ModuleLoader;

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

            variables::InfoMap variables;

            ast::StatementList startup;

        public:
            Module() = default;
            Module(std::string const& new_name,
                   std::fs::path const& new_base_path,
                   ast::StatementList const& new_code);
            Module(Module const& module) = delete;
            Module(Module&& module) = delete;
            ~Module() = default;

        public:
            Module& operator =(Module const&& module) = delete;
            Module& operator =(Module& module) = delete;

        public:
            int execute();

            void insert(typing::TypeInfoPtr&& item);

        private:
            void process_statement(ast::Statement const& statement);

            void add_sub(ast::SubDeclarationStatementPtr const& statement);
            void add_function(ast::FunctionDeclarationStatementPtr const& statement);
            void load_submodule(ast::LoadStatementPtr const& statement);
            void add_structure(ast::StructureDeclarationStatementPtr const& statemnent);
            void add_variable(ast::VariableDeclarationStatementPtr const& statement);

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
