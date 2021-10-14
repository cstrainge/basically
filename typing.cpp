
#include "basicly.h"


namespace typing
{


    TypeInfo::TypeInfo(std::string const& new_name, TypeExtraInfo new_extra)
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


    bool VariableInfo::is_array() const noexcept
    {
        return array_count != 0;
    }


    size_t VariableInfo::size() const noexcept
    {
        auto computed_size = type->size();

        if (is_array())
        {
            return array_count * computed_size;
        }

        return computed_size;
    }


}
