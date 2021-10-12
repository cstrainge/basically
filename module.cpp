
#include "basicly.h"


namespace module
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
                                   type::SignedFlag is_signed,
                                   type::FloatingPointFlag is_floating_point,
                                   size_t size)
            {
                auto extra = std::make_shared<type::NumberInfo>(is_signed,
                                                                is_floating_point,
                                                                size);

                contents.insert(std::make_shared<type::TypeInfo>(name, extra));
            };

        const auto is_signed = type::SignedFlag::IsSigned;
        const auto is_unsigned = type::SignedFlag::IsUnsigned;
        const auto is_integer = type::FloatingPointFlag::IsInteger;
        const auto is_float = type::FloatingPointFlag::IsFloatingPoint;

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


}
