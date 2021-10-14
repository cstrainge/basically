
/* -------------------------------------------------------------------------------------------------
 *
 * Basically Scripting
 *
 * ---------------------------------------------------------------------------------------------- */

#include "basically.h"


namespace
{


    void dump_tokens(std::string const& code)
    {
        std::cout << "Original source:" << std::endl
                  << code << std::endl
                  << "Found tokens:" << std::endl;

        source::Buffer source_buffer(code);
        token::Buffer token_buffer(source_buffer);

        token::Token next;

        do
        {
            next = token_buffer.next();
            std::cout << "    " << next << std::endl;
        }
        while (next.type != token::Type::Eof);
    }


    void compile_source_file(std::string const& code)
    {
        source::Buffer source_buffer(code);
        token::Buffer token_buffer(source_buffer);

        auto ast = parse::parse_to_ast(token_buffer);
    }


}


const std::string code = R"code(
# This is a starting point!

var int_val as i32 = 1024
var str_val as string = "Hello world."

function foo(x as i32) as i32
    foo = 1024 - x
end function

var x as i32 = foo(48)
)code";


int main(int argc, char* argv[])
{
    auto result = EXIT_SUCCESS;

    try
    {
        dump_tokens(code);
        std::cout << std::endl;

        compile_source_file(code);
        std::cout << "Compile finished." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        result = EXIT_FAILURE;
    }

    return result;
}
