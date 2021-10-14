
#pragma once


namespace basically::module
{


    class ModuleLoader
    {
        private:
            std::fs::path system_path;
            std::list<std::fs::path> working_path;

            ModuleMap loaded_modules;

        public:
            ModuleLoader();
            ~ModuleLoader() = default;

        public:
            void set_system_path(std::fs::path const& path);

            void push_working_path(std::fs::path const& path);
            void push_working_path_from_script(std::fs::path const& script_path);
            void pop_working_path();

        public:
            ModulePtr get_module(std::string const& name);

        private:
            bool is_readable(std::fs::path const& path) const noexcept;
    };


}
