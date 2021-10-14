
#pragma once


namespace basically::module
{


    class Module;

    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


    class Module
    {
        private:
            ModulePtr parent;
            ModuleMap children;

            typing::TypeInfoMap types;
            typing::VariableInfoMap variables;
            typing::SubInfoMap subs;
            typing::FunctionInfoMap functions;


        public:
            Module(ast::StatementList const& block);
            ~Module();

        public:
            void init_base_types();

        public:
            void insert(typing::TypeInfoPtr const& item);
            void insert(typing::VariableInfoPtr const& item);
            void insert(typing::SubInfoPtr const& item);
            void insert(typing::FunctionInfoPtr const& item);

        public:
            typing::TypeInfoPtr find_type(std::string const& name) const noexcept;
            typing::VariableInfoPtr find_variable(std::string const& name) const noexcept;
            typing::SubInfoPtr find_sub(std::string const& name) const noexcept;
            typing::FunctionInfoPtr find_function(std::string const& name) const noexcept;

        private:
            template <typename ItemType>
            ItemType find_item(std::unordered_map<std::string, ItemType> const& map,
                               std::string const& name) const noexcept;
    };


}
