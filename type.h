
#pragma once


namespace type
{


    struct NumberInfo;
    using NumberInfoPtr = std::shared_ptr<NumberInfo>;

    struct StructureInfo;
    using StructureInfoPtr = std::shared_ptr<StructureInfo>;

    using TypeExtraInfo = std::variant<NumberInfoPtr, StructureInfoPtr>;

    struct TypeInfo;
    using TypeInfoPtr = std::shared_ptr<TypeInfo>;
    using TypeInfoMap = std::unordered_map<std::string, TypeInfoPtr>;


    struct TypeInfo
    {
        std::string name;
        TypeExtraInfo extra;

        OptionalSize size() const;
    };


    struct NumberInfo
    {
        bool is_signed;
        bool is_floating_point;

        size_t size;
    };


    struct FieldInfo : public TypeInfo
    {
        TypeInfoPtr type;
        size_t offset;
        ast::ExpressionPtr initializer;
    };


    using FieldInfoList = std::vector<FieldInfo>;


    struct StructureInfo : public TypeInfo
    {
        FieldInfoList fields;
    };


    enum class Visibility
    {
        Public,
        Protected,
        Private
    };


    struct VariableInfo
    {
        std::string name;
        TypeInfoPtr type;
        Visibility visibility;

        ast::ExpressionPtr initializer;

        size_t array_count;
        bool is_const;

        bool is_array();
        size_t size();
    };


    using VariableInfoPtr = std::shared_ptr<VariableInfo>;
    using VariableInfoMap = std::unordered_map<std::string, VariableInfoPtr>;
    using VariableInfoList = std::vector<VariableInfoPtr>;


    struct SubInfo
    {
        std::string name;
        VariableInfoList parameters;
        Visibility visibility;
        ast::StatementList body;
    };


    using SubInfoPtr = std::shared_ptr<SubInfo>;
    using SubInfoMap = std::shared_ptr<SubInfoPtr>;


    struct FunctionInfo : public SubInfo
    {
        VariableInfoPtr return_type;
    };


    using FunctionInfoPtr = std::shared_ptr<FunctionInfo>;
    using FunctionInfoMap = std::shared_ptr<FunctionInfoPtr>;


    struct Namespace;
    using NamespacePtr = std::shared_ptr<Namespace>;
    using NamespaceMap = std::unordered_map<std::string, NamespacePtr>;


    struct Namespace
    {
        NamespacePtr parent;
        NamespaceMap children;

        TypeInfoMap types;
        VariableInfoMap variables;
        SubInfoMap subs;
        FunctionInfoMap functions;
    };


    class Module;
    using ModulePtr = std::shared_ptr<Module>;
    using ModuleMap = std::unordered_map<std::string, ModulePtr>;


    class Module
    {
        private:
            Namespace contents;

        public:
            Module(ast::StatementList const& block);
            ~Module();
    };


}
