
#include "basicly.h"


namespace module
{


    ModuleLoader::ModuleLoader()
    {
    }


    void ModuleLoader::set_system_path(std::string const& path)
    {
    }


    void ModuleLoader::set_working_path(std::string const& path)
    {
    }


    void ModuleLoader::set_working_path_from_script(std::string const& script_path)
    {
    }


    ModulePtr ModuleLoader::script_from_source(std::string const& source,
                                               std::string const& name) const
    {
        return nullptr;
    }


    ModulePtr ModuleLoader::script_from_stream(std::istream& stream,
                                               std::string const& name) const
    {
        return nullptr;
    }


    ModulePtr ModuleLoader::script_from_file(std::string const& file_path,
                                             std::string const& name) const
    {
        return nullptr;
    }


    ModulePtr ModuleLoader::get_module(std::string const& name)
    {
        return nullptr;
    }


}
