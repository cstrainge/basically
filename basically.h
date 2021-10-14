
#include <cstddef>
#include <optional>
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

#include <libgccjit.h>


namespace std
{


    namespace fs = filesystem;


}


using OptionalChar = std::optional<char>;
using OptionalString = std::optional<std::string>;
using OptionalSize = std::optional<size_t>;

using CompoundName = std::vector<std::string>;


#include "source.h"
#include "token.h"
#include "ast.h"
#include "parse.h"
#include "typing.h"
#include "module.h"
#include "module_loader.h"
