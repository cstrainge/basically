
/* -------------------------------------------------------------------------------------------------
 *
 * Basically Scripting
 *
 * ---------------------------------------------------------------------------------------------- */

#include "basically.h"


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
