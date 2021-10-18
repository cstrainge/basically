
#pragma once


namespace basically::ast
{


   struct Base
    {
        const source::Location location;

        Base(source::Location const& new_location)
        : location(new_location)
        {
        }

        virtual ~Base() = default;
    };


    struct ExpressionBase : public Base
    {
        ExpressionBase(source::Location const& new_location)
        : Base(new_location)
        {
        }
    };


    struct StatementBase : public Base
    {
        StatementBase(source::Location const& new_location)
        : Base(new_location)
        {
        }
    };


    struct LiteralExpression;
    struct VariableReadExpression;
    struct PrefixExpression;
    struct BinaryExpression;
    struct PostfixExpression;
    struct FunctionCallExpression;


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


    std::ostream& operator <<(std::ostream& stream, LiteralExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, VariableReadExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, PrefixExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, BinaryExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, PostfixExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, FunctionCallExpressionPtr const& expression);
    std::ostream& operator <<(std::ostream& stream, Expression const& expression);
    std::ostream& operator <<(std::ostream& stream, OptionalExpression const& expression);


    using ExpressionList = std::list<Expression>;


    struct DoStatement;
    struct ForStatement;
    struct SubDeclarationStatement;
    struct FunctionDeclarationStatement;
    struct IfStatement;
    struct LoadStatement;
    struct LoopStatement;
    struct SelectStatement;
    struct StructureDeclarationStatement;
    struct VariableDeclarationStatement;
    struct AssignmentStatement;
    struct SubCallStatement;


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


    std::ostream& operator <<(std::ostream& stream, DoStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, ForStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, SubDeclarationStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, FunctionDeclarationStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, IfStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, LoadStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, LoopStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, SelectStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, StructureDeclarationStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, VariableDeclarationStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, AssignmentStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, SubCallStatementPtr const& statement);
    std::ostream& operator <<(std::ostream& stream, VariableDeclarationList const& statement);
    std::ostream& operator <<(std::ostream& stream, Statement const& statement);
    std::ostream& operator <<(std::ostream& stream, StatementList const& statement);


    struct AstHandler
    {
        protected:
            template <typename DataType, typename HandlerType, typename DefaultHandlerType>
            void call_handler(DataType const& object,
                              HandlerType const& handler,
                              DefaultHandlerType default_handler)
            {
                if (!handler)
                {
                    assert(default_handler);
                    default_handler(object);
                }

                handler(object);
            }
    };


    #define REGISTER_HANDLER(var_name, type_name) \
        std::function<void(type_name const&)> var_name; \
        \
        void operator ()(type_name const& object) \
        { \
            call_handler(object, var_name, default_handler); \
        }


    struct ExpressionHandlers : AstHandler
    {
        REGISTER_HANDLER(literal_expression, LiteralExpressionPtr)
        REGISTER_HANDLER(variable_read_expression, VariableReadExpressionPtr)
        REGISTER_HANDLER(prefix_expression, PrefixExpressionPtr)
        REGISTER_HANDLER(binary_expression, BinaryExpressionPtr)
        REGISTER_HANDLER(postfix_expression, PostfixExpressionPtr)
        REGISTER_HANDLER(function_call_expression, FunctionCallExpressionPtr)

        std::function<void(Expression const&)> default_handler;
    };


    struct StatementHandlers : AstHandler
    {
        REGISTER_HANDLER(do_statement, DoStatementPtr)
        REGISTER_HANDLER(for_statement, ForStatementPtr)
        REGISTER_HANDLER(sub_declaration_statement, SubDeclarationStatementPtr)
        REGISTER_HANDLER(function_declaration_statement, FunctionDeclarationStatementPtr)
        REGISTER_HANDLER(if_statement, IfStatementPtr)
        REGISTER_HANDLER(load_statement, LoadStatementPtr)
        REGISTER_HANDLER(loop_statement, LoopStatementPtr)
        REGISTER_HANDLER(select_statement, SelectStatementPtr)
        REGISTER_HANDLER(structure_declaration_statement, StructureDeclarationStatementPtr)
        REGISTER_HANDLER(variable_declaration_statement, VariableDeclarationStatementPtr)
        REGISTER_HANDLER(assignment_statement, AssignmentStatementPtr)
        REGISTER_HANDLER(sub_call_statement, SubCallStatementPtr)

        std::function<void(Statement const&)> default_handler;
    };


    #undef REGISTER_HANDLER


    using ConditionalBlock = std::tuple<Expression, StatementList>;
    using ConditionalBlockList = std::list<ConditionalBlock>;


    struct LiteralExpression : public ExpressionBase
    {
        const lexing::Token value;

        LiteralExpression(lexing::Token const& new_value)
        : ExpressionBase(new_value.location),
          value(new_value)
        {
        }
    };


    struct VariableReadExpression : public ExpressionBase
    {
        const lexing::Token name;
        const Expression subscript;

        VariableReadExpression(lexing::Token const& new_name,
                               Expression const& new_subscript)
        : ExpressionBase(new_name.location),
          name(new_name),
          subscript(new_subscript)
        {
        }
    };


    struct PrefixExpression : public ExpressionBase
    {
        const lexing::Token operator_type;
        const Expression expression;

        PrefixExpression(lexing::Token const& new_operator_type,
                         Expression const& new_expression)
        : ExpressionBase(new_operator_type.location),
          operator_type(new_operator_type),
          expression(new_expression)
        {
        }
    };


    struct BinaryExpression : public ExpressionBase
    {
        const lexing::Token operator_type;
        const Expression lhs;
        const Expression rhs;

        BinaryExpression(lexing::Token const& new_operator_type,
                         Expression const& new_lhs,
                         Expression const& new_rhs)
        : ExpressionBase(new_operator_type.location),
          operator_type(new_operator_type),
          lhs(new_lhs),
          rhs(new_rhs)
        {
        }
    };


    struct PostfixExpression : public ExpressionBase
    {
        const Expression expression;
        const lexing::Token operator_type;

        PostfixExpression(Expression const& new_expression,
                          lexing::Token const& new_operator_type)
        : ExpressionBase(new_operator_type.location),
          expression(new_expression),
          operator_type(new_operator_type)
        {
        }
    };


    struct FunctionCallExpression : public ExpressionBase
    {
        const lexing::Token name;
        const ExpressionList parameters;

        FunctionCallExpression(lexing::Token const& new_name,
                               ExpressionList const& new_parameters)
        : ExpressionBase(new_name.location),
          name(new_name),
          parameters(new_parameters)
        {
        }
    };


    struct DoStatement : public StatementBase
    {
        const lexing::Token terminator;
        const Expression test;
        const StatementList body;

        DoStatement(source::Location const& new_location,
                    lexing::Token const& new_terminator,
                    Expression const& new_test,
                    StatementList const& new_body)
        : StatementBase(new_location),
          terminator(new_terminator),
          test(new_test),
          body(new_body)
        {
        }
    };


    struct ForStatement : public StatementBase
    {
        const lexing::Token index_name;
        const Expression start_index;
        const Expression end_index;
        const OptionalExpression step_value;
        const StatementList body;

        ForStatement(source::Location const& new_location,
                     lexing::Token const& new_index_name,
                     Expression const& new_start_index,
                     Expression const& new_end_index,
                     OptionalExpression const& new_step_value,
                     StatementList const& new_body)
        : StatementBase(new_location),
          index_name(new_index_name),
          start_index(new_start_index),
          end_index(new_end_index),
          step_value(new_step_value),
          body(new_body)
        {
        }
    };


    struct SubDeclarationStatement : public StatementBase
    {
        const lexing::Token name;
        const VariableDeclarationList parameters;
        const StatementList body;

        SubDeclarationStatement(source::Location const& new_location,
                                lexing::Token const& new_name,
                                VariableDeclarationList const& new_parameters,
                                StatementList const& new_body)
        : StatementBase(new_location),
          name(new_name),
          parameters(new_parameters),
          body(new_body)
        {
        }
    };


    struct FunctionDeclarationStatement : public SubDeclarationStatement
    {
        const lexing::Token return_type;

        FunctionDeclarationStatement(source::Location const& new_location,
                                     lexing::Token const& new_name,
                                     VariableDeclarationList const& new_parameters,
                                     lexing::Token const& new_return_type,
                                     StatementList const& new_body)
        : SubDeclarationStatement(new_location, new_name, new_parameters, new_body),
          return_type(new_return_type)
        {
        }
    };


    struct IfStatement : public StatementBase
    {
        const ConditionalBlock main_block;
        const ConditionalBlockList else_if_blocks;
        const StatementList else_block;

        IfStatement(source::Location const& new_location,
                    ConditionalBlock const& new_main_block,
                    ConditionalBlockList const& new_else_if_blocks,
                    StatementList const& new_else_block)
        : StatementBase(new_location),
          main_block(new_main_block),
          else_if_blocks(new_else_if_blocks),
          else_block(new_else_block)
        {
        }
    };


    struct LoadStatement : public StatementBase
    {
        const lexing::Token module_name;
        const lexing::Token alias;

        LoadStatement(source::Location const& new_location,
                      lexing::Token const& new_module_name,
                      lexing::Token const& new_alias)
        : StatementBase(new_location),
          module_name(new_module_name),
          alias(new_alias)
        {
        }
    };


    struct LoopStatement : public StatementBase
    {
        const StatementList body;

        LoopStatement(source::Location const& new_location,
                      StatementList const& new_body)
        : StatementBase(new_location),
          body(new_body)
        {
        }
    };


    struct SelectStatement : public StatementBase
    {
        const Expression test;
        const ConditionalBlockList conditions;
        const StatementList default_condition;

        SelectStatement(source::Location const& new_location,
                        Expression const& new_test,
                        ConditionalBlockList const& new_conditions,
                        StatementList const& new_default_condition)
        : StatementBase(new_location),
          test(new_test),
          conditions(new_conditions),
          default_condition(new_default_condition)
        {
        }
    };


    struct StructureDeclarationStatement : public StatementBase
    {
        const lexing::Token name;
        const VariableDeclarationList members;

        StructureDeclarationStatement(source::Location const& new_location,
                                      lexing::Token const& new_name,
                                      VariableDeclarationList const& new_members)
        : StatementBase(new_location),
          name(new_name),
          members(new_members)
        {
        }
    };


    struct VariableDeclarationStatement : public StatementBase
    {
        const lexing::Token name;
        const lexing::Token type_name;
        const OptionalExpression initializer;

        VariableDeclarationStatement(source::Location const& new_location,
                                     lexing::Token const& new_name,
                                     lexing::Token const& new_type_name,
                                     OptionalExpression const& new_initializer)
        : StatementBase(new_location),
          name(new_name),
          type_name(new_type_name),
          initializer(new_initializer)
        {
        }
    };


    struct AssignmentStatement : public StatementBase
    {
        const lexing::Token name;
        const Expression value;

        AssignmentStatement(source::Location const& new_location,
                            lexing::Token const& new_name,
                            Expression const& new_value)
        : StatementBase(new_location),
          name(new_name),
          value(new_value)
        {
        }
    };


    struct SubCallStatement : public StatementBase
    {
        const lexing::Token name;
        const ExpressionList parameters;

        SubCallStatement(source::Location const& new_location,
                         lexing::Token const& new_name,
                         ExpressionList const& new_parameters)
        : StatementBase(new_location),
          name(new_name),
          parameters(new_parameters)
        {
        }
    };


}
