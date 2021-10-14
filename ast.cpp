
#include "basically.h"


namespace basically::ast
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
                                                   ExpressionPtr const& new_subscript)
    : ExpressionBase(new_name.location),
      name(new_name),
      subscript(new_subscript)
    {
    }


    token::Token const& VariableReadExpression::get_name() const noexcept
    {
        return name;
    }


    ExpressionPtr const& VariableReadExpression::get_subscript() const noexcept
    {
        return subscript;
    }




    PrefixExpression::PrefixExpression(token::Token const& new_operator_type,
                                       ExpressionPtr const& new_expression)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      expression(new_expression)
    {
    }


    token::Token const& PrefixExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    ExpressionPtr const& PrefixExpression::get_expression() const noexcept
    {
        return expression;
    }




    BinaryExpression::BinaryExpression(token::Token const& new_operator_type,
                                       ExpressionPtr const& new_lhs,
                                       ExpressionPtr const& new_rhs)
    : ExpressionBase(new_operator_type.location),
      operator_type(new_operator_type),
      lhs(new_lhs),
      rhs(new_rhs)
    {
    }


    token::Token const& BinaryExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    ExpressionPtr const& BinaryExpression::get_lhs() const noexcept
    {
        return lhs;
    }


    ExpressionPtr const& BinaryExpression::get_rhs() const noexcept
    {
        return rhs;
    }




    PostfixExpression::PostfixExpression(ExpressionPtr const& new_expression,
                                         token::Token const& new_operator_type)
    : ExpressionBase(new_operator_type.location),
      expression(new_expression),
      operator_type(new_operator_type)
    {
    }


    ExpressionPtr const& PostfixExpression::get_expression() const noexcept
    {
        return expression;
    }


    token::Token const& PostfixExpression::get_operator_type() const noexcept
    {
        return operator_type;
    }


    FunctionCallExpression::FunctionCallExpression(token::Token const& new_name,
                                                   ExpressionList const& new_parameters)
    : ExpressionBase(new_name.location),
      name(new_name),
      parameters(new_parameters)
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
                             ExpressionPtr const& new_test,
                             StatementList const& new_body)
    : StatementBase(new_location),
      terminator(new_terminator),
      test(new_test),
      body(new_body)
    {
    }


    token::Token const& DoStatement::get_terminator() const noexcept
    {
        return terminator;
    }


    ExpressionPtr const& DoStatement::get_test() const noexcept
    {
        return test;
    }


    StatementList const& DoStatement::get_body() const noexcept
    {
        return body;
    }




    ForStatement::ForStatement(source::Location const& new_location,
                               token::Token const& new_index_name,
                               ExpressionPtr const& new_start_index,
                               ExpressionPtr const& new_end_index,
                               ExpressionPtr const& new_step_value,
                               StatementList const& new_body)
    : StatementBase(new_location),
      index_name(new_index_name),
      start_index(new_start_index),
      end_index(new_end_index),
      step_value(new_step_value),
      body(new_body)
    {
    }


    token::Token const& ForStatement::get_index_name() const noexcept
    {
        return index_name;
    }


    ExpressionPtr const& ForStatement::get_start_index() const noexcept
    {
        return start_index;
    }


    ExpressionPtr const& ForStatement::get_end_index() const noexcept
    {
        return end_index;
    }


    ExpressionPtr const& ForStatement::get_step_value() const noexcept
    {
        return step_value;
    }


    StatementList const& ForStatement::get_body() const noexcept
    {
        return body;
    }




    SubDeclarationStatement::SubDeclarationStatement(source::Location const& new_location,
                                                     token::Token const& new_name,
                                                     VariableDeclarationList const& new_parameters,
                                                     StatementList const& new_body)
    : StatementBase(new_location),
      name(new_name),
      parameters(new_parameters),
      body(new_body)
    {
    }


    token::Token const& SubDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    VariableDeclarationList const& SubDeclarationStatement::get_parameters() const noexcept
    {
        return parameters;
    }


    StatementList const& SubDeclarationStatement::get_body() const noexcept
    {
        return body;
    }




    FunctionDeclarationStatement::FunctionDeclarationStatement(
                                                      source::Location const& new_location,
                                                      token::Token const& new_name,
                                                      VariableDeclarationList const& new_parameters,
                                                      token::Token const& new_return_type,
                                                      StatementList const& new_body)
    : SubDeclarationStatement(new_location, new_name, new_parameters, new_body),
      return_type(new_return_type)
    {
    }


    token::Token const& FunctionDeclarationStatement::get_return_type() const noexcept
    {
        return return_type;
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


    ConditionalBlock const& IfStatement::get_main_block() const noexcept
    {
        return main_block;
    }


    ConditionalBlockList const& IfStatement::get_else_if_blocks() const noexcept
    {
        return else_if_blocks;
    }


    StatementList const& IfStatement::get_else_block() const noexcept
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
                                 StatementList const& new_body)
    : StatementBase(new_location),
      body(new_body)
    {
    }


    StatementList const& LoopStatement::get_body() const noexcept
    {
        return body;
    }




    SelectStatement::SelectStatement(source::Location const& new_location,
                                    ExpressionPtr const& new_test,
                                    ConditionalBlockList const& new_conditions,
                                    StatementList const& new_default_condition)
    : StatementBase(new_location),
      test(new_test),
      conditions(new_conditions),
      default_condition(new_default_condition)
    {
    }


    ExpressionPtr const& SelectStatement::get_test() const noexcept
    {
        return test;
    }


    ConditionalBlockList const& SelectStatement::get_conditions() const noexcept
    {
        return conditions;
    }


    StatementList const& SelectStatement::get_default_condition() const noexcept
    {
        return default_condition;
    }




    StructureDeclarationStatement::StructureDeclarationStatement(
                                                         source::Location const& new_location,
                                                         token::Token const& new_name,
                                                         VariableDeclarationList const& new_members)
    : StatementBase(new_location),
      name(new_name),
      members(new_members)
    {
    }


    token::Token const& StructureDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    VariableDeclarationList const& StructureDeclarationStatement::get_members() const noexcept
    {
        return members;
    }




    VariableDeclarationStatement::VariableDeclarationStatement(source::Location const& new_location,
                                                               token::Token const& new_name,
                                                               token::Token const& new_type_name,
                                                               ExpressionPtr const& new_initializer)
    : StatementBase(new_location),
      name(new_name),
      type_name(new_type_name),
      initializer(new_initializer)
    {
    }


    token::Token const& VariableDeclarationStatement::get_name() const noexcept
    {
        return name;
    }


    token::Token const& VariableDeclarationStatement::get_type_name() const noexcept
    {
        return type_name;
    }


    ExpressionPtr const& VariableDeclarationStatement::get_initilizer() const noexcept
    {
        return initializer;
    }




    AssignmentStatement::AssignmentStatement(source::Location const& new_location,
                                             token::Token const& new_name,
                                             ExpressionPtr const& new_value)
    : StatementBase(new_location),
      name(new_name),
      value(new_value)
    {
    }


    token::Token const& AssignmentStatement::get_name() const noexcept
    {
        return name;
    }


    ExpressionPtr const& AssignmentStatement::get_value() const noexcept
    {
        return value;
    }




    SubCallStatement::SubCallStatement(source::Location const& new_location,
                                       token::Token const& new_name,
                                       ExpressionList const& new_parameters)
    : StatementBase(new_location),
      name(new_name),
      parameters(new_parameters)
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
