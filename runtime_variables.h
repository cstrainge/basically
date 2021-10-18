
#pragma once


namespace basically::runtime::variables
{


    struct Info
    {
        std::string name;
        typing::TypeInfoPtr type;
        ast::Expression initializer;
        typing::Visibility visibility;
        size_t array_count;
        bool is_const;

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
        private:
            ScopePtr parent;
            InfoMap variables;

        public:
            Scope() = default;
            explicit Scope(ScopePtr& parent);
            Scope(Scope const& scope) = default;
            Scope(Scope&& scope) = default;
            ~Scope() = default;

        public:
            Scope& operator =(Scope const& scope) = default;
            Scope& operator =(Scope&& scope) = default;

        public:
            InfoPtr find(std::string const& name) const noexcept;
    };


    inline ScopePtr make_scope(ScopePtr parent = {})
    {
        return std::make_shared<Scope>(parent);
    }


}
