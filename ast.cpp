
#include "basically.h"


namespace basically::ast
{


    Base::Base(source::Location const& new_location)
    : location(new_location)
    {
    }


    ExpressionBase::ExpressionBase(source::Location const& new_location)
    : Base(new_location)
    {
    }


    StatementBase::StatementBase(source::Location const& new_location)
    : Base(new_location)
    {
    }


    LiteralExpression::LiteralExpression(lexing::Token const& new_value)
    : ExpressionBase(new_value.location),
      value(new_value)
    {
    }


    VariableReadExpression::VariableReadExpression(lexing::Token const& new_name,
                                                   Expression const& new_subscript)
    : ExpressionBase(new_name.location),
      name(new_name),
      subscript(new_subscript)
    {
    }


    PrefixExpression::PrefixExpression(lexing::Token const& new_operator_type,
                                       Expression const& new_expression)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      expression(new_expression)
    {
    }


    BinaryExpression::BinaryExpression(lexing::Token const& new_operator_type,
                                       Expression const& new_lhs,
                                       Expression const& new_rhs)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      lhs(new_lhs),
      rhs(new_rhs)
    {
    }


    PostfixExpression::PostfixExpression(Expression const& new_expression,
                                         lexing::Token const& new_operator_type)
    : ExpressionBase(new_operator_type.location),
      expression(new_expression),
      operator_type(new_operator_type)
    {
    }


    FunctionCallExpression::FunctionCallExpression(lexing::Token const& new_name,
                                                   ExpressionList const& new_parameters)
    : ExpressionBase(new_name.location),
      name(new_name),
      parameters(new_parameters)
    {
    }


    DoStatement::DoStatement(source::Location const& new_location,
                             lexing::Token const& new_terminator,
                             Expression const& new_test,
                             StatementList const& new_body)
    : StatementBase(new_location),
      terminator(new_terminator),
      test(new_test),
      body(new_body)
    {
    }


    ForStatement::ForStatement(source::Location const& new_location,
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


    SubDeclarationStatement::SubDeclarationStatement(source::Location const& new_location,
                                                     lexing::Token const& new_name,
                                                     VariableDeclarationList const& new_parameters,
                                                     StatementList const& new_body)
    : StatementBase(new_location),
      name(new_name),
      parameters(new_parameters),
      body(new_body)
    {
    }


    FunctionDeclarationStatement::FunctionDeclarationStatement(
                                                      source::Location const& new_location,
                                                      lexing::Token const& new_name,
                                                      VariableDeclarationList const& new_parameters,
                                                      lexing::Token const& new_return_type,
                                                      StatementList const& new_body)
    : SubDeclarationStatement(new_location, new_name, new_parameters, new_body),
      return_type(new_return_type)
    {
    }


    IfStatement::IfStatement(source::Location const& new_location,
                             ConditionalBlock const& new_main_block,
                             ConditionalBlockList const& new_else_if_blocks,
                             StatementList const& new_else_block)
    : StatementBase(new_location),
      main_block(new_main_block),
      else_if_blocks(new_else_if_blocks),
      else_block(new_else_block)
    {
    }


    LoadStatement::LoadStatement(source::Location const& new_location,
                                 lexing::Token const& new_module_name,
                                 lexing::Token const& new_alias)
    : StatementBase(new_location),
      module_name(new_module_name),
      alias(new_alias)
    {
    }


    LoopStatement::LoopStatement(source::Location const& new_location,
                                 StatementList const& new_body)
    : StatementBase(new_location),
      body(new_body)
    {
    }


    SelectStatement::SelectStatement(source::Location const& new_location,
                                    Expression const& new_test,
                                    ConditionalBlockList const& new_conditions,
                                    StatementList const& new_default_condition)
    : StatementBase(new_location),
      test(new_test),
      conditions(new_conditions),
      default_condition(new_default_condition)
    {
    }


    StructureDeclarationStatement::StructureDeclarationStatement(
                                                         source::Location const& new_location,
                                                         lexing::Token const& new_name,
                                                         VariableDeclarationList const& new_members)
    : StatementBase(new_location),
      name(new_name),
      members(new_members)
    {
    }


    VariableDeclarationStatement::VariableDeclarationStatement(
                                                          source::Location const& new_location,
                                                          lexing::Token const& new_name,
                                                          lexing::Token const& new_type_name,
                                                          OptionalExpression const& new_initializer)
    : StatementBase(new_location),
      name(new_name),
      type_name(new_type_name),
      initializer(new_initializer)
    {
    }


    AssignmentStatement::AssignmentStatement(source::Location const& new_location,
                                             lexing::Token const& new_name,
                                             Expression const& new_value)
    : StatementBase(new_location),
      name(new_name),
      value(new_value)
    {
    }


    SubCallStatement::SubCallStatement(source::Location const& new_location,
                                       lexing::Token const& new_name,
                                       ExpressionList const& new_parameters)
    : StatementBase(new_location),
      name(new_name),
      parameters(new_parameters)
    {
    }


}
