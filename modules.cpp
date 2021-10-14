
#include <unistd.h>

#include "basically.h"


namespace basically::modules
{


    namespace
    {


        void add_number_type(ModulePtr module,
                             std::string const& name,
                             typing::SignedFlag is_signed,
                             typing::FloatingPointFlag is_floating_point,
                             size_t size)
        {
           auto extra = std::make_shared<typing::NumberInfo>(is_signed,
                                                             is_floating_point,
                                                             size);

            module->insert(std::make_shared<typing::TypeInfo>(name,
                                                              extra,
                                                              typing::Visibility::Public));
        }


        ModulePtr create_builtins()
        {
            const auto is_signed = typing::SignedFlag::IsSigned;
            const auto is_unsigned = typing::SignedFlag::IsUnsigned;
            const auto is_integer = typing::FloatingPointFlag::IsInteger;
            const auto is_float = typing::FloatingPointFlag::IsFloatingPoint;

            auto builtins = std::make_shared<Module>();

            add_number_type(builtins, "i8", is_signed, is_integer, 1);
            add_number_type(builtins, "u8", is_unsigned, is_integer, 1);

            add_number_type(builtins, "i16", is_signed, is_integer, 2);
            add_number_type(builtins, "u16", is_unsigned, is_integer, 2);

            add_number_type(builtins, "i32", is_signed, is_integer, 4);
            add_number_type(builtins, "u32", is_unsigned, is_integer, 4);

            add_number_type(builtins, "i64", is_signed, is_integer, 8);
            add_number_type(builtins, "u64", is_unsigned, is_integer, 8);

            add_number_type(builtins, "f32", is_signed, is_float, 4);
            add_number_type(builtins, "f64", is_signed, is_float, 8);

            return builtins;
        }


    }


    ModulePtr& get_builtins_module()
    {
        static ModulePtr builtins = create_builtins();
        return builtins;
    }



    Module::Module(ast::StatementList const& block)
    {
    }


    int Module::execute()
    {
        return 0;
    }


    void Module::insert(typing::TypeInfoPtr const& item)
    {
        types[item->name] = item;
    }


    Loader::Loader()
    {
    }


    void Loader::set_system_path(std::fs::path const& path)
    {
        system_path = path;
    }


    void Loader::push_working_path(std::fs::path const& path)
    {
        auto status = std::fs::status(path);

        if (!std::fs::exists(status))
        {
            throw std::runtime_error("Missing working directory, " + path.string() + ".");
        }

        if (!std::fs::is_directory(status))
        {
            throw std::runtime_error("Tried to set the working path to a non-directory," +
                                     path.string() + ".");
        }

        if (!is_readable(path))
        {
            throw std::runtime_error("Can not read from working path, " + path.string() + ".");
        }

        working_path.push_front(path);
    }


//    void Loader::push_working_path_from_script(std::fs::path const& script_path)
//    {
//        std::fs::path new_path = script_path;
//        push_working_path(new_path.remove_filename());
//    }


    void Loader::pop_working_path()
    {
        assert(!working_path.empty());
        working_path.pop_front();
    }


    ModulePtr Loader::get_script(std::fs::path const& script_path)
    {
        return nullptr;
    }


    ModulePtr Loader::get_module(std::string const& name)
    {
        return nullptr;
    }



    bool Loader::is_readable(std::fs::path const& path) const noexcept
    {
        return access(path.c_str(), R_OK) != -1;
    }


}
