
/* -------------------------------------------------------------------------------------------------
 *
 * Basicly Scripting
 *
 * ---------------------------------------------------------------------------------------------- */

#include "std_inc.h"
#include "source.h"
#include "token.h"
#include "ast.h"
#include "parse.h"


namespace
{


    void dump_tokens(std::string const& code)
    {
        source::Buffer source_buffer(code);
        token::Buffer token_buffer(source_buffer);

        token::Token next;

        do
        {
            next = token_buffer.next();
            std::cout << next << std::endl;
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

structure teh_datas
    a as i32
    b as i32
    c as string
end structure

var int_val as i32
var float_val as i64

# int_val = 10

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
