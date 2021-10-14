
#include "basically.h"


namespace basically::typing
{


    TypeInfo::TypeInfo(std::string const& new_name,
                       TypeExtraInfo new_extra,
                       Visibility visibility)
    : name(new_name),
      extra(new_extra)
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
                            size_sum = field.type->size();
                        }

                        new_size = size_sum;
                    }
            };

        std::visit(handler, extra);

        return new_size;
    }


}
