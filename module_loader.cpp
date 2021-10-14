
#include <unistd.h>

#include "basically.h"


namespace module
{


    ModuleLoader::ModuleLoader()
    {
    }


    void ModuleLoader::set_system_path(std::fs::path const& path)
    {
        system_path = path;
    }


    void ModuleLoader::push_working_path(std::fs::path const& path)
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


    void ModuleLoader::push_working_path_from_script(std::fs::path const& script_path)
    {
        std::fs::path new_path = script_path;
        push_working_path(new_path.remove_filename());
    }


    void ModuleLoader::pop_working_path()
    {
        if (working_path.empty())
        {
            throw std::runtime_error("Internal error, working path stack misalignment.");
        }

        working_path.pop_front();
    }


    ModulePtr ModuleLoader::get_module(std::string const& name)
    {
        return nullptr;
    }



    bool ModuleLoader::is_readable(std::fs::path const& path) const noexcept
    {
        return access(path.c_str(), R_OK) != -1;
    }


}
