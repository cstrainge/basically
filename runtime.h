
#pragma once


namespace basically::runtime
{


    using PartList = std::vector<std::string>;


    struct CompoundName
    {
        const PartList parts;

        CompoundName(std::string const& dotted_name);
    };


    std::strong_ordering operator <=>(CompoundName const& lhs, CompoundName const& rhs);
    std::strong_ordering operator <=>(std::string const& lhs, CompoundName const& rhs);
    std::strong_ordering operator <=>(CompoundName const& lhs, std::string const& rhs);


}
