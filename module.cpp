
#include "basically.h"


namespace basically::module
{


    Module::Module(ast::StatementList const& block)
    {
    }


    Module::~Module()
    {
    }


    void Module::init_base_types()
    {
        auto add_number_type = [&](std::string const& name,
                                   typing::SignedFlag is_signed,
                                   typing::FloatingPointFlag is_floating_point,
                                   size_t size)
            {
                auto extra = std::make_shared<typing::NumberInfo>(is_signed,
                                                                is_floating_point,
                                                                size);

                insert(std::make_shared<typing::TypeInfo>(name, extra));
            };

        const auto is_signed = typing::SignedFlag::IsSigned;
        const auto is_unsigned = typing::SignedFlag::IsUnsigned;
        const auto is_integer = typing::FloatingPointFlag::IsInteger;
        const auto is_float = typing::FloatingPointFlag::IsFloatingPoint;

        add_number_type("i8", is_signed, is_integer, 1);
        add_number_type("u8", is_unsigned, is_integer, 1);

        add_number_type("i16", is_signed, is_integer, 2);
        add_number_type("u16", is_unsigned, is_integer, 2);

        add_number_type("i32", is_signed, is_integer, 4);
        add_number_type("u32", is_unsigned, is_integer, 4);

        add_number_type("i64", is_signed, is_integer, 8);
        add_number_type("u64", is_unsigned, is_integer, 8);

        add_number_type("f32", is_signed, is_float, 4);
        add_number_type("f64", is_signed, is_float, 8);
    }


    void Module::insert(typing::TypeInfoPtr const& item)
    {
        types[item->name] = item;
    }


    void Module::insert(typing::VariableInfoPtr const& item)
    {
        variables[item->name] = item;
    }


    void Module::insert(typing::SubInfoPtr const& item)
    {
        subs[item->name] = item;
    }


    void Module::insert(typing::FunctionInfoPtr const& item)
    {
        functions[item->name] = item;
    }


    typing::TypeInfoPtr Module::find_type(std::string const& name) const noexcept
    {
        return find_item(types, name);
    }


    typing::VariableInfoPtr Module::find_variable(std::string const& name) const noexcept
    {
        return find_item(variables, name);
    }


    typing::SubInfoPtr Module::find_sub(std::string const& name) const noexcept
    {
        return find_item(subs, name);
    }


    typing::FunctionInfoPtr Module::find_function(std::string const& name) const noexcept
    {
        return find_item(functions, name);
    }


    template <typename ItemType>
    ItemType Module::find_item(std::unordered_map<std::string, ItemType> const& map,
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
