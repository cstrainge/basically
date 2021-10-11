
#pragma once


namespace type
{


    struct Type;

    using TypePtr = std::shared_ptr<Type>;
    using TypeList = std::vector<TypePtr>;


    struct Type
    {
        std::string name;
        size_t size;
        TypeList fields;
        ast::ExpressionPtr initializer;
    };


    using TypePtr = std::shared_ptr<Type>;


    class Module
    {
        public:
            Module(ast::StatementList const& block);
            ~Module();
    };


}
