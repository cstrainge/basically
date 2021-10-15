
/* -------------------------------------------------------------------------------------------------
 *
 * Basically Scripting
 *
 * ---------------------------------------------------------------------------------------------- */

#include "basically.h"


namespace
{


    std::fs::path get_system_path(std::fs::path const& executable)
    {
        std::fs::path base_path = executable.is_relative()
            ? std::fs::absolute(executable)
            : executable;

        return base_path.remove_filename().lexically_normal();
    }


}


int main(int argc, char* argv[])
{
    auto result = EXIT_SUCCESS;

    try
    {
        if (argc != 2)
        {
            throw std::runtime_error("Need to specifiy a script to run.");
        }

        basically::modules::Loader loader;

        loader.set_system_path(get_system_path(argv[0]));
        auto script = loader.get_script(argv[1]);

        result = script->execute();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        result = EXIT_FAILURE;
    }

    return result;
}
