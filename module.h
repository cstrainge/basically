
#pragma once


namespace module
{


    class Module
    {
        private:
            type::Namespace contents;

        public:
            Module(ast::StatementList const& block);
            ~Module();

        public:
            void init_base_types();
    };


    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


}
