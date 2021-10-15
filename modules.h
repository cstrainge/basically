
#pragma once


namespace basically::modules
{


    class Module;

    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


    ModulePtr& get_builtins_module();


    class Module
    {
        private:
            ModuleMap loaded_modules;

            typing::TypeInfoMap types;

            variables::InfoMap variables;

            typing::SubInfoMap subs;
            typing::FunctionInfoMap functions;

            ast::StatementList startup;

        public:
            Module() = default;
            Module(Module const& module) = delete;
            Module(Module&& module) = delete;
            Module(ast::StatementList const& block);
            ~Module() = default;

        public:
            Module& operator =(Module const&& module) = delete;
            Module& operator =(Module& module) = delete;

        public:
            int execute();

        public:
            void insert(typing::TypeInfoPtr const& item);
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
