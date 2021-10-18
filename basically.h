
#ifndef BASICLY_INC_H
#define BASICLY_INC_H


    #include <cstddef>
    #include <optional>
    #include <iterator>
    #include <string>
    #include <iostream>
    #include <sstream>
    #include <fstream>
    #include <unordered_set>
    #include <unordered_map>
    #include <vector>
    #include <list>
    #include <tuple>
    #include <memory>
    #include <functional>
    #include <stdexcept>
    #include <compare>
    #include <variant>
    #include <filesystem>
    #include <cassert>

    #include <unistd.h>
    #include <libgccjit.h>


    namespace std
    {


        namespace fs = filesystem;


    }


    namespace basically
    {


        using OptionalChar = std::optional<char>;
        using OptionalString = std::optional<std::string>;
        using OptionalSize = std::optional<size_t>;
        using OptionalPath = std::optional<std::fs::path>;


    }


    #include "source.h"
    #include "lexing.h"
    #include "ast.h"
    #include "parsing.h"
    #include "typing.h"
    #include "runtime.h"
    #include "runtime_variables.h"
    #include "runtime_modules.h"


    namespace basically
    {

        inline int execute_script(std::fs::path const& system_path,
                                  std::fs::path const& script_path)
        {
            runtime::modules::Loader loader;

            loader.set_system_path(system_path);

            //auto script_path = std::fs::canonical(script.is_relative() ? std::fs::absolute(script)
            //                                                           : script);

            auto loaded_script = loader.get_script(script_path);
            return loaded_script->execute();

        }

    }


#endif
