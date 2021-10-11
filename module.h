
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
    };


    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


}
