
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

        bool is_array() const noexcept;
        size_t size() const noexcept;
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
    using SubInfoMap = std::unordered_map<std::string, SubInfoPtr>;


    struct FunctionInfo : public SubInfo
    {
        VariableInfoPtr return_type;
    };


    using FunctionInfoPtr = std::shared_ptr<FunctionInfo>;
    using FunctionInfoMap = std::unordered_map<std::string, FunctionInfoPtr>;


    class Namespace;
    using NamespacePtr = std::shared_ptr<Namespace>;
    using NamespaceMap = std::unordered_map<std::string, NamespacePtr>;


    class Namespace
    {
        private:
            NamespacePtr parent;
            NamespaceMap children;

            TypeInfoMap types;
            VariableInfoMap variables;
            SubInfoMap subs;
            FunctionInfoMap functions;

        public:
            Namespace() = default;
            ~Namespace() = default;

        public:
            void insert(TypeInfoPtr const& item);
            void insert(VariableInfoPtr const& item);
            void insert(SubInfoPtr const& item);
            void insert(FunctionInfoPtr const& item);

        public:
            TypeInfoPtr find_type(std::string const& name) const noexcept;
            VariableInfoPtr find_variable(std::string const& name) const noexcept;
            SubInfoPtr find_sub(std::string const& name) const noexcept;
            FunctionInfoPtr find_function(std::string const& name) const noexcept;

        private:
            template <typename ItemType>
            ItemType find_item(std::unordered_map<std::string, ItemType> const& map,
                                std::string const& name) const noexcept;
    };


}
