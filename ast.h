
#pragma once


namespace ast
{


   class Base
    {
        private:
            source::Location location;

        public:
            Base(source::Location const& new_location);
            Base(Base const& ast) = default;
            Base(Base&& ast) = default;
            virtual ~Base() = default;

        public:
            Base& operator =(Base const& ast) = default;
            Base& operator =(Base&& ast) = default;

        public:
            source::Location const& get_loctation() const noexcept;
    };


    class ExpressionBase : public Base
    {
        public:
            ExpressionBase(source::Location const& new_location);
            ExpressionBase(ExpressionBase const& expression) = default;
            ExpressionBase(ExpressionBase&& expression) = default;
            virtual ~ExpressionBase() = default;

        public:
            ExpressionBase& operator =(ExpressionBase const& expression) = default;
            ExpressionBase& operator =(ExpressionBase&& expression) = default;
    };


    class StatementBase : public Base
    {
        public:
            StatementBase(source::Location const& new_location);
            StatementBase(StatementBase const& statement) = default;
            StatementBase(StatementBase&& statement) = default;
            virtual ~StatementBase() = default;

        public:
            StatementBase& operator =(StatementBase const& statement) = default;
            StatementBase& operator =(StatementBase&& statement) = default;
    };


    using Expression = std::shared_ptr<ExpressionBase>;
    using ExpressionList = std::list<Expression>;


    using Statement = std::shared_ptr<StatementBase>;
    using StatementBlock = std::list<Statement>;


    using ConditionalBlock = std::tuple<Expression, StatementBlock>;
    using ConditionalBlockList = std::list<ConditionalBlock>;


    class VariableDeclarationStatement;

    using VariableDeclarationStatementPtr = std::shared_ptr<VariableDeclarationStatement>;
    using VariableDeclarationList = std::list<VariableDeclarationStatementPtr>;


    class LiteralExpression : public ExpressionBase
    {
        private:
            token::Token value;

        public:
            LiteralExpression(token::Token const& value);

        public:
            token::Token const& get_value() const noexcept;
    };


    class VariableReadExpression : public ExpressionBase
    {
        private:
            token::Token name;
            Expression subscript;

        public:
            VariableReadExpression(token::Token const& new_name,
                                  Expression const& new_subscript);

        public:
            token::Token const& get_name() const noexcept;
            Expression const& get_subscript() const noexcept;
    };


    class PrefixExpression : public ExpressionBase
    {
        private:
            token::Token operator_type;
            Expression expression;

        public:
            PrefixExpression(token::Token const& new_operator_type,
                             Expression const& new_expression);

        public:
            token::Token const& get_operator_type() const noexcept;
            Expression const& get_expression() const noexcept;
    };


    class BinaryExpression : public ExpressionBase
    {
        private:
            token::Token operator_type;
            Expression lhs;
            Expression rhs;

        public:
            BinaryExpression(token::Token const& new_operator_type,
                             Expression const& new_lhs,
                             Expression const& new_rhs);

        public:
            token::Token const& get_operator_type() const noexcept;
            Expression const& get_lhs() const noexcept;
            Expression const& get_rhs() const noexcept;
    };


    class PostfixExpression : public ExpressionBase
    {
        private:
            Expression expression;
            token::Token operator_type;

        public:
            PostfixExpression(Expression const& new_expression,
                              token::Token const& new_operator_type);

        public:
            Expression const& get_expression() const noexcept;
            token::Token const& get_operator_type() const noexcept;
    };


    class FunctionCallExpression : public ExpressionBase
    {
        private:
            token::Token name;
            ExpressionList parameters;

        public:
            FunctionCallExpression(token::Token const& new_name,
                                   ExpressionList const& new_parameters);

        public:
            token::Token const& get_name() const noexcept;
            ExpressionList const& get_parameters() const noexcept;
    };


    class DoStatement : public StatementBase
    {
        private:
            token::Token terminator;
            Expression test;
            StatementBlock body;

        public:
            DoStatement(source::Location const& new_location,
                        token::Token const& new_terminator,
                        Expression const& new_test,
                        StatementBlock const& new_body);

        public:
            token::Token const& get_terminator() const noexcept;
            Expression const& get_test() const noexcept;
            StatementBlock const& get_body() const noexcept;
    };


    class ForStatement : public StatementBase
    {
        private:
            token::Token index_name;
            Expression start_index;
            Expression end_index;
            Expression step_value;
            StatementBlock body;

        public:
            ForStatement(source::Location const& new_location,
                         token::Token const& new_index_name,
                         Expression const& new_start_index,
                         Expression const& new_end_index,
                         Expression const& new_step_value,
                         StatementBlock const& new_body);

        public:
            token::Token const& get_index_name() const noexcept;
            Expression const& get_start_index() const noexcept;
            Expression const& get_end_index() const noexcept;
            Expression const& get_step_value() const noexcept;
            StatementBlock const& get_body() const noexcept;
    };


    class SubDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            VariableDeclarationList parameters;
            StatementBlock body;

        public:
            SubDeclarationStatement(source::Location const& new_location,
                                    token::Token const& name,
                                    VariableDeclarationList const& parameters,
                                    StatementBlock const& body);

        public:
            token::Token const& get_name() const noexcept;
            VariableDeclarationList const& get_parameters() const noexcept;
            StatementBlock const& get_body() const noexcept;
    };


    class FunctionDeclarationStatement : public SubDeclarationStatement
    {
        private:
            token::Token return_type;

        public:
            FunctionDeclarationStatement(source::Location const& new_location,
                                         token::Token const& name,
                                         VariableDeclarationList const& parameters,
                                         token::Token const& return_type,
                                         StatementBlock const& body);

        public:
            token::Token const& get_return_type() const noexcept;
    };


    class IfStatement : public StatementBase
    {
        private:
            ConditionalBlock main_block;
            ConditionalBlockList else_if_blocks;
            StatementBlock else_block;

        public:
            IfStatement(source::Location const& new_location,
                        ConditionalBlock const& new_main_block,
                        ConditionalBlockList const& new_else_if_blocks,
                        StatementBlock const& new_else_block);

        public:
            ConditionalBlock const& get_main_block() const noexcept;
            ConditionalBlockList const& get_else_if_blocks() const noexcept;
            StatementBlock const& get_else_block() const noexcept;
    };


    class LoadStatement : public StatementBase
    {
        private:
            token::Token module_name;

        public:
            LoadStatement(source::Location const& new_location,
                          token::Token new_module_name);

        public:
            token::Token const& get_module_name() const noexcept;
    };


    class LoopStatement : public StatementBase
    {
        private:
            StatementBlock body;

        public:
            LoopStatement(source::Location const& new_location,
                          StatementBlock const& new_body);

        public:
            StatementBlock const& get_body() const noexcept;
    };


    class SelectStatement : public StatementBase
    {
        private:
            Expression test;
            ConditionalBlockList conditions;
            StatementBlock default_condition;

        public:
            SelectStatement(source::Location const& new_location,
                            Expression const& new_test,
                            ConditionalBlockList const& new_conditions,
                            StatementBlock const& new_default_condition);

        public:
            Expression const& get_test() const noexcept;
            ConditionalBlockList const& get_conditions() const noexcept;
            StatementBlock const& get_default_condition() const noexcept;
    };


    class StructureDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            VariableDeclarationList members;

        public:
            StructureDeclarationStatement(source::Location const& new_location,
                                          token::Token const& new_name,
                                          VariableDeclarationList const& new_members);

        public:
            token::Token const& get_name() const noexcept;
            VariableDeclarationList const& get_members() const noexcept;
    };


    class VariableDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            token::Token type_name;
            Expression initilizer;

        public:
            VariableDeclarationStatement(source::Location const& new_location,
                                         token::Token const& new_name,
                                         token::Token const& new_type_name,
                                         Expression const& new_initilizer);

        public:
            token::Token const& get_name() const noexcept;
            token::Token const& get_type_name() const noexcept;
            Expression const& get_initilizer() const noexcept;
    };


    class AssignmentStatement : public StatementBase
    {
        private:
            token::Token name;
            Expression value;

        public:
            AssignmentStatement(source::Location const& new_location,
                                token::Token const& new_name,
                                Expression const& new_value);

        public:
            token::Token const& get_name() const noexcept;
            Expression const& get_value() const noexcept;
    };


    class SubCallStatement : public StatementBase
    {
        private:
            token::Token name;
            ExpressionList parameters;

        public:
            SubCallStatement(source::Location const& new_location,
                             token::Token const& new_name,
                             ExpressionList const& new_parameters);

        public:
            token::Token const& get_name() const noexcept;
            ExpressionList const& get_parameters() const noexcept;
    };


}
