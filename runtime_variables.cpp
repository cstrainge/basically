
#include "basically.h"


namespace basically::runtime::variables
{


    Info::Info(ast::VariableDeclarationStatementPtr const& declaration)
    /*: name(declaration->name.text),
      type(declaration->type_name),
      initializer(declaration->initializer),
      array_count(0),
      is_const(false),
      visibility(typing::Visibility::Default)*/
    {
    }


    bool Info::is_array() const noexcept
    {
        return array_count != 0;
    }


    size_t Info::size() const noexcept
    {
        assert(type.resolved_type);

        auto computed_size = type.resolved_type->size();

        if (is_array())
        {
            return array_count * computed_size;
        }

        return computed_size;
    }


}
