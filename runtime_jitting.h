
#pragma once


namespace basically::runtime::jitting
{


    struct Options
    {
        OptionalString progname;

        OptionalBool debug_info = true;

        OptionalBool dump_initial_tree;
        OptionalBool dump_initial_gimple;
        OptionalBool dump_generated_code;
        OptionalBool dump_summary;
        OptionalBool dump_everything;
        OptionalBool selfcheck_gc;
        OptionalBool keep_intermediates;
    };


    class Jit
    {
        private:
            gcc_jit_context* context;
            gcc_jit_result* result;

        public:
            Jit();
            Jit(Options const& options);
            Jit(Jit const& jit) = delete;
            Jit(Jit&& jit) noexcept;
            ~Jit() noexcept;

        public:
            Jit& operator =(Jit const& jit) = delete;
            Jit& operator =(Jit&& jit) noexcept;

        public:
            // create_type

            void create_local(variables::Info variable);

            // create_sub
            // create_function

        private:
            void release() noexcept;
    };


}
