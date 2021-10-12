
#include "basicly.h"


namespace type
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


    void Namespace::insert(TypeInfoPtr const& item)
    {
        types[item->name] = item;
    }


    void Namespace::insert(VariableInfoPtr const& item)
    {
        variables[item->name] = item;
    }


    void Namespace::insert(SubInfoPtr const& item)
    {
        subs[item->name] = item;
    }


    void Namespace::insert(FunctionInfoPtr const& item)
    {
        functions[item->name] = item;
    }


    TypeInfoPtr Namespace::find_type(std::string const& name) const noexcept
    {
        return find_item(types, name);
    }


    VariableInfoPtr Namespace::find_variable(std::string const& name) const noexcept
    {
        return find_item(variables, name);
    }


    SubInfoPtr Namespace::find_sub(std::string const& name) const noexcept
    {
        return find_item(subs, name);
    }


    FunctionInfoPtr Namespace::find_function(std::string const& name) const noexcept
    {
        return find_item(functions, name);
    }


    template <typename ItemType>
    ItemType Namespace::find_item(std::unordered_map<std::string, ItemType> const& map,
                                   std::string const& name) const noexcept
    {
        if (auto found_item = map.find(name); found_item != map.end())
        {
            return found_item->second;
        }

        if (parent)
        {
            return parent->find_item(map, name);
        }

        return nullptr;
    }


}
