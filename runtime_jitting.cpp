
#include "basically.h"


namespace basically::runtime::jitting
{


    namespace
    {


        template <typename IdType>
        void call_setter(gcc_jit_context* context, IdType id, OptionalBool const& option) noexcept
        {
            gcc_jit_context_set_bool_option(context, id, option.value());
        }


        template <typename IdType>
        void call_setter(gcc_jit_context* context, IdType id, OptionalString const& option) noexcept
        {
            gcc_jit_context_set_str_option(context, id, option.value().c_str());
        }


        template <typename IdType>
        void call_setter(gcc_jit_context* context, IdType id, OptionalInt const& option) noexcept
        {
            gcc_jit_context_set_int_option(context, id, option.value());
        }


        template <typename IdType, typename ValueType>
        void set_if_provided(gcc_jit_context* context,
                             IdType id,
                             ValueType const& option) noexcept
        {
            if (option.has_value())
            {
                call_setter(context, id, option);
            }
        }


        #define SET_OPTION(OPTION_ID, OPTION_VALUE) \
            set_if_provided(context, OPTION_ID, options.OPTION_VALUE);


        void set_context_options(gcc_jit_context* context, Options const& options) noexcept
        {
            SET_OPTION(GCC_JIT_STR_OPTION_PROGNAME, progname);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DEBUGINFO, debug_info);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DUMP_INITIAL_TREE, dump_initial_tree);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DUMP_INITIAL_GIMPLE, dump_initial_gimple);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DUMP_GENERATED_CODE, dump_generated_code);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DUMP_SUMMARY, dump_summary);
            SET_OPTION(GCC_JIT_BOOL_OPTION_DUMP_EVERYTHING, dump_everything);
            SET_OPTION(GCC_JIT_BOOL_OPTION_SELFCHECK_GC, selfcheck_gc);
            SET_OPTION(GCC_JIT_BOOL_OPTION_KEEP_INTERMEDIATES, keep_intermediates);
            SET_OPTION(GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL, optimization_level);
        }


        #undef SET_OPTION


    }


    Jit::Jit()
    : Jit(Options {})
    {
        if (context == nullptr)
        {
            throw std::runtime_error("Could not aquire JIT context.");
        }
    }


    Jit::Jit(Options const& options)
    : context(gcc_jit_context_acquire()),
      result(nullptr)
    {
        set_context_options(context, options);
    }


    Jit::Jit(Jit&& jit) noexcept
    : context(jit.context),
      result(jit.result)
    {
        jit.context = nullptr;
        jit.result = nullptr;
    }


    Jit::~Jit() noexcept
    {
        release();
    }



    Jit& Jit::operator =(Jit&& jit) noexcept
    {
        if (&jit != this)
        {
            release();

            context = jit.context;
            result = jit.result;

            jit.context = nullptr;
            jit.result = nullptr;
        }

        return *this;
    }



    void Jit::release() noexcept
    {
        if (context != nullptr)
        {
            gcc_jit_context_release(context);
            context = nullptr;
        }

        if (result != nullptr)
        {
            gcc_jit_result_release(result);
            result = nullptr;
        }
    }


}
