
#include "basically.h"


namespace basically::variables
{


    bool Info::is_array() const noexcept
    {
        return array_count != 0;
    }


    size_t Info::size() const noexcept
    {
        auto computed_size = type->size();

        if (is_array())
        {
            return array_count * computed_size;
        }

        return computed_size;
    }


}
