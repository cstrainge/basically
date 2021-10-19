
#include "basically.h"


namespace basically::typing
{


    TypeRef::TypeRef(lexing::Token const& ref_token)
    : ref_location(ref_token.location),
      type_name(ref_token.text),
      resolved_type(nullptr)
    {
        assert(ref_token.type == lexing::Type::Identifier);
    }


    TypeInfo::TypeInfo(ast::StructureDeclarationStatementPtr const& declaration)
    : TypeInfo(declaration->name.text, std::make_shared<StructureInfo>(declaration))
      // visibility
    {
    }


    TypeInfo::TypeInfo(std::string const& new_name,
                       TypeExtraInfo new_extra,
                       Visibility new_visibility)
    : name(new_name),
      extra(new_extra),
      visibility(new_visibility)
    {
    }


    NumberInfo::NumberInfo(SignedFlag new_is_signed,
                           FloatingPointFlag new_is_floating_point,
                           size_t new_size) noexcept
    : is_signed(new_is_signed),
      is_floating_point(new_is_floating_point),
      size(new_size)
    {
    }


    size_t TypeInfo::size() const noexcept
    {
        size_t new_size;

        ExtraInfoHandler handler =
            {
                .number = [&](auto const& value)
                    {
                        new_size = value->size;
                    },

                .structure = [&](auto const& value)
                    {
                        size_t size_sum = 0;

                        for (auto const& field : value->fields)
                        {
                            size_sum = field.type.resolved_type->size();
                        }

                        new_size = size_sum;
                    }
            };

        std::visit(handler, extra);

        return new_size;
    }


    StructureInfo::StructureInfo(ast::StructureDeclarationStatementPtr const& declaration)
    {
    }


    SubInfo::SubInfo(ast::SubDeclarationStatementPtr const& declaration)
    : name(declaration->name.text),
      parameters(),
      body()
    {
    }


    FunctionInfo::FunctionInfo(ast::FunctionDeclarationStatementPtr const& declaration)
    : SubInfo(declaration),
      return_type(declaration->return_type)
    {
    }


}
