
#include "basicly.h"


namespace ast
{


    Base::Base(source::Location const& new_location)
    : location(new_location)
    {
    }


    source::Location const& Base::get_loctation() const noexcept
    {
        return location;
    }




    ExpressionBase::ExpressionBase(source::Location const& new_location)
    : Base(new_location)
    {
    }




    StatementBase::StatementBase(source::Location const& new_location)
    : Base(new_location)
    {
    }



    LiteralExpression::LiteralExpression(token::Token const& new_value)
    : ExpressionBase(new_value.location),
      value(new_value)
    {
    }


    token::Token const& LiteralExpression::get_value() const noexcept
    {
        return value;
    }




    VariableReadExpression::VariableReadExpression(token::Token const& new_name,
                                                   Expression&& new_subscript)
    : ExpressionBase(new_name.location),
      name(new_name),
      subscript(std::move(new_subscript))
    {
    }


    token::Token const& VariableReadExpression::get_name() const noexcept
    {
        return name;
    }


    Expression const& VariableReadExpression::get_subscript() const noexcept
    {
        return subscript;
    }




    PrefixExpression::PrefixExpression(token::Token const& new_operator_type,
                                       Expression&& new_expression)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      expression(std::move(new_expression))
    {
    }


    token::Token const& PrefixExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    Expression const& PrefixExpression::get_expression() const noexcept
    {
        return expression;
    }




    BinaryExpression::BinaryExpression(token::Token const& new_operator_type,
                        Expression&& new_lhs,
                        Expression&& new_rhs)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      lhs(std::move(new_lhs)),
      rhs(std::move(new_rhs))
    {
    }


    token::Token const& BinaryExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    Expression const& BinaryExpression::get_lhs() const noexcept
    {
        return lhs;
    }


    Expression const& BinaryExpression::get_rhs() const noexcept
    {
        return rhs;
    }




    PostfixExpression::PostfixExpression(Expression&& new_expression,
                                         token::Token const& new_operator_type)
    : ExpressionBase(new_operator_type.location),
      expression(std::move(new_expression)),
      operator_type(new_operator_type)
    {
    }


    Expression const& PostfixExpression::get_expression() const noexcept
    {
        return expression;
    }


    token::Token const& PostfixExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    FunctionCallExpression::FunctionCallExpression(token::Token const& new_name,
                                                   ExpressionList&& new_parameters)
    : ExpressionBase(new_name.location),
      name(new_name),
      parameters(std::move(new_parameters))
    {
    }

    token::Token const& FunctionCallExpression::get_name() const noexcept
    {
        return name;
    }


    ExpressionList const& FunctionCallExpression::get_parameters() const noexcept
    {
        return parameters;
    }




    DoStatement::DoStatement(source::Location const& new_location,
                             token::Token const& new_terminator,
                             Expression&& new_test,
                             StatementBlock&& new_body)
    : StatementBase(new_location),
      terminator(new_terminator),
      test(std::move(new_test)),
      body(std::move(new_body))
    {
    }


    token::Token const& DoStatement::get_terminator() const noexcept
    {
        return terminator;
    }


    Expression const& DoStatement::get_test() const noexcept
    {
        return test;
    }


    StatementBlock const& DoStatement::get_body() const noexcept
    {
        return body;
    }




    ForStatement::ForStatement(source::Location const& new_location,
                    token::Token const& new_index_name,
                    Expression&& new_start_index,
                    Expression&& new_end_index,
                    Expression&& new_step_value,
                    StatementBlock&& new_body)
    : StatementBase(new_location),
      index_name(std::move(new_index_name)),
      start_index(std::move(new_start_index)),
      end_index(std::move(new_end_index)),
      step_value(std::move(new_step_value)),
      body(std::move(new_body))
    {
    }


    token::Token const& ForStatement::get_index_name() const noexcept
    {
        return index_name;
    }


    Expression const& ForStatement::get_start_index() const noexcept
    {
        return start_index;
    }


    Expression const& ForStatement::get_end_index() const noexcept
    {
        return end_index;
    }


    Expression const& ForStatement::get_step_value() const noexcept
    {
        return step_value;
    }


    StatementBlock const& ForStatement::get_body() const noexcept
    {
        return body;
    }




    SubDeclarationStatement::SubDeclarationStatement(source::Location const& new_location,
                                                     token::Token const& new_name,
                                                     VaraibleDeclarationList&& new_parameters,
                                                     StatementBlock&& new_body)
    : StatementBase(new_location),
      name(new_name),
      parameters(std::move(new_parameters)),
      body(std::move(new_body))
    {
    }


    token::Token const& SubDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    VaraibleDeclarationList const& SubDeclarationStatement::get_parameters() const noexcept
    {
        return parameters;
    }


    StatementBlock const& SubDeclarationStatement::get_body() const noexcept
    {
        return body;
    }




    FunctionDeclarationStatement::FunctionDeclarationStatement(
                                                           source::Location const& new_location,
                                                           token::Token const& new_name,
                                                           VaraibleDeclarationList&& new_parameters,
                                                           token::Token const& new_return_type,
                                                           StatementBlock&& new_body)
    : SubDeclarationStatement(new_location,
                              new_name,
                              std::move(new_parameters),
                              std::move(new_body)),
      return_type(new_return_type)
    {
    }


    token::Token const& FunctionDeclarationStatement::get_return_type() const noexcept
    {
        return return_type;
    }




    IfStatement::IfStatement(source::Location const& new_location,
                             ConditionalBlock&& new_main_block,
                             ConditionalBlockList&& new_else_if_blocks,
                             StatementBlock&& new_else_block)
    : StatementBase(new_location),
      main_block(std::move(new_main_block)),
      else_if_blocks(std::move(new_else_if_blocks)),
      else_block(std::move(new_else_block))
    {
    }


    ConditionalBlock const& IfStatement::get_main_block() const noexcept
    {
        return main_block;
    }


    ConditionalBlockList const& IfStatement::get_else_if_blocks() const noexcept
    {
        return else_if_blocks;
    }


    StatementBlock const& IfStatement::get_else_block() const noexcept
    {
        return else_block;
    }




    LoadStatement::LoadStatement(source::Location const& new_location,
                                 token::Token new_module_name)
    : StatementBase(new_location),
      module_name(new_module_name)
    {
    }


    token::Token const& LoadStatement::get_module_name() const noexcept
    {
        return module_name;
    }




    LoopStatement::LoopStatement(source::Location const& new_location,
                                 StatementBlock&& new_body)
    : StatementBase(new_location),
      body(std::move(new_body))
    {
    }


    StatementBlock const& LoopStatement::get_body() const noexcept
    {
        return body;
    }




    SelectStatement::SelectStatement(source::Location const& new_location,
                                    Expression&& new_test,
                                    ConditionalBlockList&& new_conditions,
                                    StatementBlock&& new_default_condition)
    : StatementBase(new_location),
      test(std::move(new_test)),
      conditions(std::move(new_conditions)),
      default_condition(std::move(new_default_condition))
    {
    }


    Expression const& SelectStatement::get_test() const noexcept
    {
        return test;
    }


    ConditionalBlockList const& SelectStatement::get_conditions() const noexcept
    {
        return conditions;
    }


    StatementBlock const& SelectStatement::get_default_condition() const noexcept
    {
        return default_condition;
    }




    StructureDeclarationStatement::StructureDeclarationStatement(source::Location const& new_location,
                                                                 token::Token const& new_name,
                                                                 VaraibleDeclarationList&& new_members)
    : StatementBase(new_location),
      name(new_name),
      members(std::move(new_members))
    {
    }


    token::Token const& StructureDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    VaraibleDeclarationList const& StructureDeclarationStatement::get_members() const noexcept
    {
        return members;
    }




    VaraibleDeclarationStatement::VaraibleDeclarationStatement(source::Location const& new_location,
                                                               token::Token const& new_name,
                                                               token::Token const& new_type_name,
                                                               Expression&& new_initilizer)
    : StatementBase(new_location),
      name(new_name),
      type_name(new_type_name),
      initilizer(std::move(new_initilizer))
    {
    }


    token::Token const& VaraibleDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    token::Token const& VaraibleDeclarationStatement::get_type_name() const noexcept
    {
        return type_name;
    }


    Expression const& VaraibleDeclarationStatement::get_initilizer() const noexcept
    {
        return initilizer;
    }




    AssignmentStatement::AssignmentStatement(source::Location const& new_location,
                                             token::Token const& new_name,
                                             Expression&& new_value)
    : StatementBase(new_location),
      name(new_name),
      value(std::move(new_value))
    {
    }


    token::Token const& AssignmentStatement::get_name() const noexcept
    {
        return name;
    }


    Expression const& AssignmentStatement::get_value() const noexcept
    {
        return value;
    }




    SubCallStatement::SubCallStatement(source::Location const& new_location,
                                       token::Token const& new_name,
                                       ExpressionList&& new_parameters)
    : StatementBase(new_location),
      name(new_name),
      parameters(std::move(new_parameters))
    {
    }

    token::Token const& SubCallStatement::get_name() const noexcept
    {
        return name;
    }


    ExpressionList const& SubCallStatement::get_parameters() const noexcept
    {
        return parameters;
    }



}
