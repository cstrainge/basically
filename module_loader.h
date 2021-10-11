
#pragma once


namespace module
{


    class ModuleLoader
    {
        private:
            std::string system_path;
            std::string working_path;

            ModuleMap loaded_modules;

        public:
            ModuleLoader();
            ~ModuleLoader() = default;

        public:
            void set_system_path(std::string const& path);

            void set_working_path(std::string const& path);
            void set_working_path_from_script(std::string const& script_path);

        public:
            ModulePtr script_from_source(std::string const& source,
                                         std::string const& name = ":memory:") const;
            ModulePtr script_from_stream(std::istream& stream,
                                         std::string const& name = ":stream:") const;
            ModulePtr script_from_file(std::string const& file_path,
                                       std::string const& name = "") const;

        public:
            ModulePtr get_module(std::string const& name);
    };


}
