
#pragma once


namespace basically::ast
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


    using ExpressionPtr = std::shared_ptr<ExpressionBase>;
    using ExpressionList = std::list<ExpressionPtr>;


    using StatementPtr = std::shared_ptr<StatementBase>;
    using StatementList = std::list<StatementPtr>;


    using ConditionalBlock = std::tuple<ExpressionPtr, StatementList>;
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
            ExpressionPtr subscript;

        public:
            VariableReadExpression(token::Token const& new_name,
                                   ExpressionPtr const& new_subscript);

        public:
            token::Token const& get_name() const noexcept;
            ExpressionPtr const& get_subscript() const noexcept;
    };


    class PrefixExpression : public ExpressionBase
    {
        private:
            token::Token operator_type;
            ExpressionPtr expression;

        public:
            PrefixExpression(token::Token const& new_operator_type,
                             ExpressionPtr const& new_expression);

        public:
            token::Token const& get_operator_type() const noexcept;
            ExpressionPtr const& get_expression() const noexcept;
    };


    class BinaryExpression : public ExpressionBase
    {
        private:
            token::Token operator_type;
            ExpressionPtr lhs;
            ExpressionPtr rhs;

        public:
            BinaryExpression(token::Token const& new_operator_type,
                             ExpressionPtr const& new_lhs,
                             ExpressionPtr const& new_rhs);

        public:
            token::Token const& get_operator_type() const noexcept;
            ExpressionPtr const& get_lhs() const noexcept;
            ExpressionPtr const& get_rhs() const noexcept;
    };


    class PostfixExpression : public ExpressionBase
    {
        private:
            ExpressionPtr expression;
            token::Token operator_type;

        public:
            PostfixExpression(ExpressionPtr const& new_expression,
                              token::Token const& new_operator_type);

        public:
            ExpressionPtr const& get_expression() const noexcept;
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
            ExpressionPtr test;
            StatementList body;

        public:
            DoStatement(source::Location const& new_location,
                        token::Token const& new_terminator,
                        ExpressionPtr const& new_test,
                        StatementList const& new_body);

        public:
            token::Token const& get_terminator() const noexcept;
            ExpressionPtr const& get_test() const noexcept;
            StatementList const& get_body() const noexcept;
    };


    class ForStatement : public StatementBase
    {
        private:
            token::Token index_name;
            ExpressionPtr start_index;
            ExpressionPtr end_index;
            ExpressionPtr step_value;
            StatementList body;

        public:
            ForStatement(source::Location const& new_location,
                         token::Token const& new_index_name,
                         ExpressionPtr const& new_start_index,
                         ExpressionPtr const& new_end_index,
                         ExpressionPtr const& new_step_value,
                         StatementList const& new_body);

        public:
            token::Token const& get_index_name() const noexcept;
            ExpressionPtr const& get_start_index() const noexcept;
            ExpressionPtr const& get_end_index() const noexcept;
            ExpressionPtr const& get_step_value() const noexcept;
            StatementList const& get_body() const noexcept;
    };


    class SubDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            VariableDeclarationList parameters;
            StatementList body;

        public:
            SubDeclarationStatement(source::Location const& new_location,
                                    token::Token const& name,
                                    VariableDeclarationList const& parameters,
                                    StatementList const& body);

        public:
            token::Token const& get_name() const noexcept;
            VariableDeclarationList const& get_parameters() const noexcept;
            StatementList const& get_body() const noexcept;
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
                                         StatementList const& body);

        public:
            token::Token const& get_return_type() const noexcept;
    };


    class IfStatement : public StatementBase
    {
        private:
            ConditionalBlock main_block;
            ConditionalBlockList else_if_blocks;
            StatementList else_block;

        public:
            IfStatement(source::Location const& new_location,
                        ConditionalBlock const& new_main_block,
                        ConditionalBlockList const& new_else_if_blocks,
                        StatementList const& new_else_block);

        public:
            ConditionalBlock const& get_main_block() const noexcept;
            ConditionalBlockList const& get_else_if_blocks() const noexcept;
            StatementList const& get_else_block() const noexcept;
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
            StatementList body;

        public:
            LoopStatement(source::Location const& new_location,
                          StatementList const& new_body);

        public:
            StatementList const& get_body() const noexcept;
    };


    class SelectStatement : public StatementBase
    {
        private:
            ExpressionPtr test;
            ConditionalBlockList conditions;
            StatementList default_condition;

        public:
            SelectStatement(source::Location const& new_location,
                            ExpressionPtr const& new_test,
                            ConditionalBlockList const& new_conditions,
                            StatementList const& new_default_condition);

        public:
            ExpressionPtr const& get_test() const noexcept;
            ConditionalBlockList const& get_conditions() const noexcept;
            StatementList const& get_default_condition() const noexcept;
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
            ExpressionPtr initializer;

        public:
            VariableDeclarationStatement(source::Location const& new_location,
                                         token::Token const& new_name,
                                         token::Token const& new_type_name,
                                         ExpressionPtr const& new_initializer);

        public:
            token::Token const& get_name() const noexcept;
            token::Token const& get_type_name() const noexcept;
            ExpressionPtr const& get_initilizer() const noexcept;
    };


    class AssignmentStatement : public StatementBase
    {
        private:
            token::Token name;
            ExpressionPtr value;

        public:
            AssignmentStatement(source::Location const& new_location,
                                token::Token const& new_name,
                                ExpressionPtr const& new_value);

        public:
            token::Token const& get_name() const noexcept;
            ExpressionPtr const& get_value() const noexcept;
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
