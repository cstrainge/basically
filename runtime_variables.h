
#pragma once


namespace basically::runtime::variables
{


    struct Info
    {
        std::string name;
        typing::TypeRef type;

        ast::OptionalExpression initializer;

        size_t array_count;
        bool is_const;

        typing::Visibility visibility;

        Info(std::string const& new_name,
             std::string const& new_type_name,
             size_t new_array_count,
             bool new_is_const);
        Info(ast::VariableDeclarationStatementPtr const& declaration);

        bool is_array() const noexcept;
        size_t size() const noexcept;
    };


    using InfoPtr = std::shared_ptr<Info>;

    using InfoMap = std::unordered_map<std::string, InfoPtr>;
    using InfoList = std::vector<InfoPtr>;


    class Scope;
    using ScopePtr = std::shared_ptr<Scope>;


    class Scope
    {
        public:
            ScopePtr parent;
            InfoMap variables;

        public:
            Scope() = default;
            Scope(ScopePtr& parent);
            Scope(Scope const& scope) = default;
            Scope(Scope&& scope) = default;
            ~Scope() = default;

        public:
            Scope& operator =(Scope const& scope) = default;
            Scope& operator =(Scope&& scope) = default;

        public:
            InfoPtr find(std::string const& name) const noexcept;
            void insert(InfoPtr const& variable);
    };


    inline ScopePtr make_scope(ScopePtr parent = {})
    {
        return std::make_shared<Scope>(parent);
    }


}
