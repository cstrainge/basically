
#include "basically.h"


namespace basically::runtime
{


    namespace
    {


        PartList split(std::string const& dotted_name)
        {
            std::istringstream stream(dotted_name);
            PartList parts;
            std::string part;

            while (std::getline(stream, part, '.'))
            {
                parts.push_back(part);
            }

            return parts;
        }


    }



    CompoundName::CompoundName(std::string const& dotted_name)
    : parts(split(dotted_name))
    {
    }


    std::strong_ordering operator <=>(CompoundName const& lhs, CompoundName const& rhs)
    {
        if (auto size_compare = lhs.parts.size() <=> lhs.parts.size();
            size_compare != std::strong_ordering::equal)
        {
            return size_compare;
        }

        const auto size = lhs.parts.size();
        std::strong_ordering result = std::strong_ordering::equal;

        for (auto i = 0; i < size; ++i)
        {
            result = lhs.parts[i] <=> rhs.parts[i];

            if (result != std::strong_ordering::equal)
            {
                break;
            }
        }

        return result;
    }


    std::strong_ordering operator <=>(std::string const& lhs, CompoundName const& rhs)
    {
        if (rhs.parts.empty())
        {
            return lhs <=> "";
        }

        if (rhs.parts.size() > 1)
        {
            return std::strong_ordering::less;
        }

        return lhs <=> rhs.parts[0];
    }


    std::strong_ordering operator <=>(CompoundName const& lhs, std::string const& rhs)
    {
        if (lhs.parts.empty())
        {
            return "" <=> rhs;
        }

        if (lhs.parts.size() > 1)
        {
            return std::strong_ordering::greater;
        }

        return lhs.parts[0] <=> rhs;
    }


}
