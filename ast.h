
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
            ~ExpressionBase() override = default;

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
            ~StatementBase() override = default;

        public:
            StatementBase& operator =(StatementBase const& statement) = default;
            StatementBase& operator =(StatementBase&& statement) = default;
    };


    class LiteralExpression;
    class VariableReadExpression;
    class PrefixExpression;
    class BinaryExpression;
    class PostfixExpression;
    class FunctionCallExpression;

    using LiteralExpressionPtr = std::shared_ptr<LiteralExpression>;
    using VariableReadExpressionPtr = std::shared_ptr<VariableReadExpression>;
    using PrefixExpressionPtr = std::shared_ptr<PrefixExpression>;
    using BinaryExpressionPtr = std::shared_ptr<BinaryExpression>;
    using PostfixExpressionPtr = std::shared_ptr<PostfixExpression>;
    using FunctionCallExpressionPtr = std::shared_ptr<FunctionCallExpression>;


    using Expression = std::variant<LiteralExpressionPtr,
                                    VariableReadExpressionPtr,
                                    PrefixExpressionPtr,
                                    BinaryExpressionPtr,
                                    PostfixExpressionPtr,
                                    FunctionCallExpressionPtr>;

    using OptionalExpression = std::optional<Expression>;

    using ExpressionList = std::list<Expression>;


    struct ExpressionHandler
    {
        std::function<void(LiteralExpressionPtr const&)> literal_expression;
        std::function<void(VariableReadExpressionPtr const&)> variable_read_expression;
        std::function<void(PrefixExpressionPtr const&)> prefix_expression;
        std::function<void(BinaryExpressionPtr const&)> binary_expression;
        std::function<void(PostfixExpressionPtr const&)> postfix_expression;
        std::function<void(FunctionCallExpressionPtr const&)> function_call_expression;

        void operator ()(LiteralExpressionPtr const& expression)
        {
            assert(literal_expression);
            literal_expression(expression);
        }

        void operator ()(VariableReadExpressionPtr const& expression)
        {
            assert(variable_read_expression);
            variable_read_expression(expression);
        }

        void operator ()(PrefixExpressionPtr const& expression)
        {
            assert(prefix_expression);
            prefix_expression(expression);
        }

        void operator ()(BinaryExpressionPtr const& expression)
        {
            assert(binary_expression);
            binary_expression(expression);
        }

        void operator ()(PostfixExpressionPtr const& expression)
        {
            assert(postfix_expression);
            postfix_expression(expression);
        }

        void operator ()(FunctionCallExpressionPtr const& expression)
        {
            assert(function_call_expression);
            function_call_expression(expression);
        }
    };


    class DoStatement;
    class ForStatement;
    class SubDeclarationStatement;
    class FunctionDeclarationStatement;
    class IfStatement;
    class LoadStatement;
    class LoopStatement;
    class SelectStatement;
    class StructureDeclarationStatement;
    class VariableDeclarationStatement;
    class AssignmentStatement;
    class SubCallStatement;

    using DoStatementPtr = std::shared_ptr<DoStatement>;
    using ForStatementPtr = std::shared_ptr<ForStatement>;
    using SubDeclarationStatementPtr = std::shared_ptr<SubDeclarationStatement>;
    using FunctionDeclarationStatementPtr = std::shared_ptr<FunctionDeclarationStatement>;
    using IfStatementPtr = std::shared_ptr<IfStatement>;
    using LoadStatementPtr = std::shared_ptr<LoadStatement>;
    using LoopStatementPtr = std::shared_ptr<LoopStatement>;
    using SelectStatementPtr = std::shared_ptr<SelectStatement>;
    using StructureDeclarationStatementPtr = std::shared_ptr<StructureDeclarationStatement>;
    using VariableDeclarationStatementPtr = std::shared_ptr<VariableDeclarationStatement>;
    using AssignmentStatementPtr = std::shared_ptr<AssignmentStatement>;
    using SubCallStatementPtr = std::shared_ptr<SubCallStatement>;

    using VariableDeclarationList = std::list<VariableDeclarationStatementPtr>;


    using Statement = std::variant<DoStatementPtr,
                                   ForStatementPtr,
                                   SubDeclarationStatementPtr,
                                   FunctionDeclarationStatementPtr,
                                   IfStatementPtr,
                                   LoadStatementPtr,
                                   LoopStatementPtr,
                                   SelectStatementPtr,
                                   StructureDeclarationStatementPtr,
                                   VariableDeclarationStatementPtr,
                                   AssignmentStatementPtr,
                                   SubCallStatementPtr>;

    using StatementList = std::list<Statement>;


    struct StatementHandler
    {
        std::function<void(DoStatementPtr const&)> do_statement;
        std::function<void(ForStatementPtr const&)> for_statement;
        std::function<void(SubDeclarationStatementPtr const&)> sub_declaration_statement;
        std::function<void(FunctionDeclarationStatementPtr const&)> function_declaration_statement;
        std::function<void(IfStatementPtr const&)> if_statement;
        std::function<void(LoadStatementPtr const&)> load_statement;
        std::function<void(LoopStatementPtr const&)> loop_statement;
        std::function<void(SelectStatementPtr const&)> select_statement;
        std::function<void(StructureDeclarationStatementPtr const&)> structure_declaration_statement;
        std::function<void(VariableDeclarationStatementPtr const&)> variable_declaration_statement;
        std::function<void(AssignmentStatementPtr const&)> assignment_statement;
        std::function<void(SubCallStatementPtr const&)> sub_call_statement;
    };


    using ConditionalBlock = std::tuple<Expression, StatementList>;
    using ConditionalBlockList = std::list<ConditionalBlock>;


    class LiteralExpression : public ExpressionBase
    {
        private:
            token::Token value;

        public:
            LiteralExpression() = default;
            LiteralExpression(token::Token const& value);
            LiteralExpression(LiteralExpression const& expression) = default;
            LiteralExpression(LiteralExpression&& expression) = default;
            ~LiteralExpression() override = default;

        public:
            LiteralExpression& operator =(LiteralExpression const& expression) = default;
            LiteralExpression& operator =(LiteralExpression&& expression) = default;

        public:
            token::Token const& get_value() const noexcept;
    };


    class VariableReadExpression : public ExpressionBase
    {
        private:
            token::Token name;
            Expression subscript;

        public:
            VariableReadExpression() = default;
            VariableReadExpression(token::Token const& new_name,
                                   Expression const& new_subscript);
            VariableReadExpression(VariableReadExpression const& expression) = default;
            VariableReadExpression(VariableReadExpression&& expression) = default;
            ~VariableReadExpression() override = default;

        public:
            VariableReadExpression& operator =(VariableReadExpression const& expression) = default;
            VariableReadExpression& operator =(VariableReadExpression&& expression) = default;

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
            PrefixExpression() = default;
            PrefixExpression(token::Token const& new_operator_type,
                             Expression const& new_expression);
            PrefixExpression(PrefixExpression const& expression) = default;
            PrefixExpression(PrefixExpression&& expression) = default;
            ~PrefixExpression() override = default;

        public:
            PrefixExpression& operator =(PrefixExpression const& expression) = default;
            PrefixExpression& operator =(PrefixExpression&& expression) = default;

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
            BinaryExpression() = default;
            BinaryExpression(token::Token const& new_operator_type,
                             Expression const& new_lhs,
                             Expression const& new_rhs);
            BinaryExpression(BinaryExpression const& expression) = default;
            BinaryExpression(BinaryExpression&& expression) = default;
            ~BinaryExpression() override = default;

        public:
            BinaryExpression& operator =(BinaryExpression const& expression) = default;
            BinaryExpression& operator =(BinaryExpression&& expression) = default;

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
            PostfixExpression() = default;
            PostfixExpression(Expression const& new_expression,
                              token::Token const& new_operator_type);
            PostfixExpression(PostfixExpression const& expression) = default;
            PostfixExpression(PostfixExpression&& expression) = default;
            ~PostfixExpression() override = default;

        public:
            PostfixExpression& operator =(PostfixExpression const& expression) = default;
            PostfixExpression& operator =(PostfixExpression&& expression) = default;

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
            FunctionCallExpression() = default;
            FunctionCallExpression(token::Token const& new_name,
                                   ExpressionList const& new_parameters);
            FunctionCallExpression(FunctionCallExpression const& expression) = default;
            FunctionCallExpression(FunctionCallExpression&& expression) = default;
            ~FunctionCallExpression() override = default;

        public:
            FunctionCallExpression& operator =(FunctionCallExpression const& expression) = default;
            FunctionCallExpression& operator =(FunctionCallExpression&& expression) = default;

        public:
            token::Token const& get_name() const noexcept;
            ExpressionList const& get_parameters() const noexcept;
    };


    class DoStatement : public StatementBase
    {
        private:
            token::Token terminator;
            Expression test;
            StatementList body;

        public:
            DoStatement() = default;
            DoStatement(source::Location const& new_location,
                        token::Token const& new_terminator,
                        Expression const& new_test,
                        StatementList const& new_body);
            DoStatement(DoStatement const& statement) = default;
            DoStatement(DoStatement&& statement) = default;
            ~DoStatement() override = default;

        public:
            DoStatement& operator =(DoStatement const& statement) = default;
            DoStatement& operator =(DoStatement&& statement) = default;

        public:
            token::Token const& get_terminator() const noexcept;
            Expression const& get_test() const noexcept;
            StatementList const& get_body() const noexcept;
    };


    class ForStatement : public StatementBase
    {
        private:
            token::Token index_name;
            Expression start_index;
            Expression end_index;
            OptionalExpression step_value;
            StatementList body;

        public:
            ForStatement() = default;
            ForStatement(source::Location const& new_location,
                         token::Token const& new_index_name,
                         Expression const& new_start_index,
                         Expression const& new_end_index,
                         OptionalExpression const& new_step_value,
                         StatementList const& new_body);
            ForStatement(ForStatement const& statement) = default;
            ForStatement(ForStatement&& statement) = default;
            ~ForStatement() override = default;

        public:
            ForStatement& operator =(ForStatement const& statement) = default;
            ForStatement& operator =(ForStatement&& statement) = default;

        public:
            token::Token const& get_index_name() const noexcept;
            Expression const& get_start_index() const noexcept;
            Expression const& get_end_index() const noexcept;
            OptionalExpression const& get_step_value() const noexcept;
            StatementList const& get_body() const noexcept;
    };


    class SubDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            VariableDeclarationList parameters;
            StatementList body;

        public:
            SubDeclarationStatement() = default;
            SubDeclarationStatement(source::Location const& new_location,
                                    token::Token const& name,
                                    VariableDeclarationList const& parameters,
                                    StatementList const& body);
            SubDeclarationStatement(SubDeclarationStatement const& statement) = default;
            SubDeclarationStatement(SubDeclarationStatement&& statement) = default;
            ~SubDeclarationStatement() override = default;

        public:
            SubDeclarationStatement& operator =(SubDeclarationStatement const& statement) = default;
            SubDeclarationStatement& operator =(SubDeclarationStatement&& statement) = default;

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
            FunctionDeclarationStatement() = default;
            FunctionDeclarationStatement(source::Location const& new_location,
                                         token::Token const& name,
                                         VariableDeclarationList const& parameters,
                                         token::Token const& return_type,
                                         StatementList const& body);
            FunctionDeclarationStatement(FunctionDeclarationStatement const& statement) = default;
            FunctionDeclarationStatement(FunctionDeclarationStatement&& statement) = default;
            ~FunctionDeclarationStatement() override = default;

        public:
            FunctionDeclarationStatement& operator =(FunctionDeclarationStatement const& statement) = default;
            FunctionDeclarationStatement& operator =(FunctionDeclarationStatement&& statement) = default;

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
            IfStatement() = default;
            IfStatement(source::Location const& new_location,
                        ConditionalBlock const& new_main_block,
                        ConditionalBlockList const& new_else_if_blocks,
                        StatementList const& new_else_block);
            IfStatement(IfStatement const& statement) = default;
            IfStatement(IfStatement&& statement) = default;
            ~IfStatement() override = default;

        public:
            IfStatement& operator =(IfStatement const& statement) = default;
            IfStatement& operator =(IfStatement&& statement) = default;

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
            LoadStatement() = default;
            LoadStatement(source::Location const& new_location,
                          token::Token new_module_name);
            LoadStatement(LoadStatement const& statement) = default;
            LoadStatement(LoadStatement&& statement) = default;
            ~LoadStatement() override = default;

        public:
            LoadStatement& operator =(LoadStatement const& statement) = default;
            LoadStatement& operator =(LoadStatement&& statement) = default;

        public:
            token::Token const& get_module_name() const noexcept;
    };


    class LoopStatement : public StatementBase
    {
        private:
            StatementList body;

        public:
            LoopStatement() = default;
            LoopStatement(source::Location const& new_location,
                          StatementList const& new_body);
            LoopStatement(LoopStatement const& statement) = default;
            LoopStatement(LoopStatement&& statement) = default;
            ~LoopStatement() override = default;

        public:
            LoopStatement& operator =(LoopStatement const& statement) = default;
            LoopStatement& operator =(LoopStatement&& statement) = default;

        public:
            StatementList const& get_body() const noexcept;
    };


    class SelectStatement : public StatementBase
    {
        private:
            Expression test;
            ConditionalBlockList conditions;
            StatementList default_condition;

        public:
            SelectStatement() = default;
            SelectStatement(source::Location const& new_location,
                            Expression const& new_test,
                            ConditionalBlockList const& new_conditions,
                            StatementList const& new_default_condition);
            SelectStatement(SelectStatement const& statement) = default;
            SelectStatement(SelectStatement&& statement) = default;
            ~SelectStatement() override = default;

        public:
            SelectStatement& operator =(SelectStatement const& statement) = default;
            SelectStatement& operator =(SelectStatement&& statement) = default;

        public:
            Expression const& get_test() const noexcept;
            ConditionalBlockList const& get_conditions() const noexcept;
            StatementList const& get_default_condition() const noexcept;
    };


    class StructureDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            VariableDeclarationList members;

        public:
            StructureDeclarationStatement() = default;
            StructureDeclarationStatement(source::Location const& new_location,
                                          token::Token const& new_name,
                                          VariableDeclarationList const& new_members);
            StructureDeclarationStatement(StructureDeclarationStatement const& statement) = default;
            StructureDeclarationStatement(StructureDeclarationStatement&& statement) = default;
            ~StructureDeclarationStatement() override = default;

        public:
            StructureDeclarationStatement& operator =(StructureDeclarationStatement const& statement) = default;
            StructureDeclarationStatement& operator =(StructureDeclarationStatement&& statement) = default;

        public:
            token::Token const& get_name() const noexcept;
            VariableDeclarationList const& get_members() const noexcept;
    };


    class VariableDeclarationStatement : public StatementBase
    {
        private:
            token::Token name;
            token::Token type_name;
            OptionalExpression initializer;

        public:
            VariableDeclarationStatement() = default;
            VariableDeclarationStatement(source::Location const& new_location,
                                         token::Token const& new_name,
                                         token::Token const& new_type_name,
                                         OptionalExpression const& new_initializer);
            VariableDeclarationStatement(VariableDeclarationStatement const& statement) = default;
            VariableDeclarationStatement(VariableDeclarationStatement&& statement) = default;
            ~VariableDeclarationStatement() override = default;

        public:
            VariableDeclarationStatement& operator =(VariableDeclarationStatement const& statement) = default;
            VariableDeclarationStatement& operator =(VariableDeclarationStatement&& statement) = default;

        public:
            token::Token const& get_name() const noexcept;
            token::Token const& get_type_name() const noexcept;
            OptionalExpression const& get_initilizer() const noexcept;
    };


    class AssignmentStatement : public StatementBase
    {
        private:
            token::Token name;
            Expression value;

        public:
            AssignmentStatement() = default;
            AssignmentStatement(source::Location const& new_location,
                                token::Token const& new_name,
                                Expression const& new_value);
            AssignmentStatement(AssignmentStatement const& statement) = default;
            AssignmentStatement(AssignmentStatement&& statement) = default;
            ~AssignmentStatement() override = default;

        public:
            AssignmentStatement& operator =(AssignmentStatement const& statement) = default;
            AssignmentStatement& operator =(AssignmentStatement&& statement) = default;

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
            SubCallStatement() = default;
            SubCallStatement(source::Location const& new_location,
                             token::Token const& new_name,
                             ExpressionList const& new_parameters);
            SubCallStatement(SubCallStatement const& statement) = default;
            SubCallStatement(SubCallStatement&& statement) = default;
            ~SubCallStatement() override = default;

        public:
            SubCallStatement& operator =(SubCallStatement const& statement) = default;
            SubCallStatement& operator =(SubCallStatement&& statement) = default;

        public:
            token::Token const& get_name() const noexcept;
            ExpressionList const& get_parameters() const noexcept;
    };


}
