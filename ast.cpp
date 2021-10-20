
#include "basically.h"


namespace basically::ast
{


    namespace
    {


        struct Indent
        {
            size_t amount = 0;

            Indent& operator ++()
            {
                amount += 4;
                return *this;
            }

            Indent& operator --()
            {
                assert(amount >= 4);
                amount -= 4;
                return *this;
            }
        };


        std::ostream& operator <<(std::ostream& stream, Indent const& the_indent)
        {
            std::string indent_str(the_indent.amount, ' ');

            stream << indent_str;

            return stream;
        }


        thread_local Indent indent;


    }


    std::ostream& operator <<(std::ostream& stream, LiteralExpressionPtr const& expression)
    {
        stream << "'"
               << expression->value.text
               << "'";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, VariableReadExpressionPtr const& expression)
    {
        stream << expression->name.text;

        //if (expression->subscript)
        //{
        //    stream << "[" << expression->subscript << "]";
        //}

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, PrefixExpressionPtr const& expression)
    {
        stream << expression->operator_type << " " << expression;

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, BinaryExpressionPtr const& expression)
    {
        std::unordered_map<lexing::Type, std::string> type_map =
            {
                { lexing::Type::SymbolEqual,       "==" },
                { lexing::Type::SymbolPlus,        "+"  },
                { lexing::Type::SymbolMinus,       "-"  },
                { lexing::Type::SymbolTimes,       "*"  },
                { lexing::Type::SymbolDivide,      "/"  },
                { lexing::Type::SymbolLessThan,    "<"  },
                { lexing::Type::SymbolGreaterThan, ">"  },
                { lexing::Type::SymbolNotEqual,    "<>" }
            };

        auto to_string = [&](auto type) -> std::string
            {
                if (auto found = type_map.find(type); found != type_map.end())
                {
                    return found->second;
                }

                return "???";
            };

        stream << "(" << expression->lhs << " "
                      << to_string(expression->operator_type.type) << " "
                      << expression->rhs << ")";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, PostfixExpressionPtr const& expression)
    {
        stream << expression->expression << expression->operator_type;

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, FunctionCallExpressionPtr const& expression)
    {
        stream << expression->name.text << "(" << expression->parameters << ")";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, Expression const& expression)
    {
        std::visit(ExpressionHandlers
            {
                .literal_expression       = [&](auto literal)  { stream << literal;  },
                .variable_read_expression = [&](auto read)     { stream << read;     },
                .prefix_expression        = [&](auto prefix)   { stream << prefix;   },
                .binary_expression        = [&](auto binary)   { stream << binary;   },
                .postfix_expression       = [&](auto postifix) { stream << postifix; },
                .function_call_expression = [&](auto call)     { stream << call;     }
            },
            expression);

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, OptionalExpression const& expression)
    {
        stream << "?{";

        if (expression)
        {
            stream << expression.value();
        }

        stream << "}";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, ExpressionList const& expressions)
    {
        auto size = expressions.size();

        for (size_t i = 0; i < expressions.size(); ++i)
        {
            stream << expressions[i];

            if (i < (size - 1))
            {
                stream << ", ";
            }
        }

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, DoStatementPtr const& statement)
    {
        auto terminator_to_string = [&]()
            {
                auto type = statement->terminator.type;

                assert(   (type == lexing::Type::KeywordWhile)
                       || (type == lexing::Type::KeywordUntil));

                return type == lexing::Type::KeywordWhile ? "while" : "until";
            };


        stream << "do " << terminator_to_string() << " " << statement->test << std::endl;

        ++indent;
        stream << statement->body;
        --indent;

        stream << indent << "end do";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, ForStatementPtr const& statement)
    {
        stream << "for " << statement->index_name.text
               << " = " << statement->start_index
               << " to " << statement->end_index
               << " step " << statement->step_value
               << std::endl;

        ++indent;
        stream << indent << statement->body << std::endl;
        --indent;

        stream << indent << "end for";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, SubDeclarationStatementPtr const& statement)
    {
        stream << "sub " << statement->name.text
               << "( " << statement->parameters << ")"
               << std::endl;

        ++indent;
        stream << statement->body << std::endl;
        --indent;

        stream << indent << "end sub";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream,
                              FunctionDeclarationStatementPtr const& statement)
    {
        stream << "function " << statement->name.text
               << "( " << statement->parameters << ") as "
               << statement->return_type.text
               << std::endl;

        ++indent;
        stream << statement->body << std::endl;
        --indent;

        stream << indent << "end function";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, IfStatementPtr const& statement)
    {
        auto [ main_test, main_body ] = statement->main_block;

        ++indent;
        stream << "if " << main_test << " then" << std::endl
               << indent << main_body << std::endl;
        --indent;

        for (auto& else_if : statement->else_if_blocks)
        {
            auto [ test, body ] = else_if;

            stream << indent << "else if " << test << " then" << std::endl;

            ++indent;
            stream << indent << body << std::endl;
            --indent;
        }

        if (!statement->else_block.empty())
        {
            stream << indent << "else" << std::endl;

            ++indent;
            stream << indent << statement->else_block << std::endl;
            --indent;
        }

        stream << indent << "end if";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, LoadStatementPtr const& statement)
    {
        stream << "load " << statement->module_name.text;

        if (statement->alias.type != lexing::Type::None)
        {
            stream << " as " << statement->alias;
        }

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, LoopStatementPtr const& statement)
    {
        ++indent;
        stream << "loop" << std::endl << indent << statement->body;
        --indent;

        stream << indent << "end loop";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, SelectStatementPtr const& statement)
    {
        stream << "select " << statement->test << std::endl;

        ++indent;

        for (auto const& condition : statement->conditions)
        {
            auto const& [ test, body ] = condition;

            stream << indent << "case " << test << std::endl;

            if (!body.empty())
            {
                ++indent;
                stream << indent << body << std::endl;
                --indent;
            }
        }

        if (!statement->default_condition.empty())
        {
            stream << indent << "else" << std::endl;

            ++indent;
            stream << indent << statement->default_condition << std::endl;
            --indent;
        }

        --indent;

        stream << indent << "end select";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream,
                              StructureDeclarationStatementPtr const& statement)
    {
        stream << "structure " << statement->name << std::endl;

        ++indent;

        for (auto const& field : statement->members)
        {
            stream << indent << field->name.text << " as " << field->type_name.text;

            if (field->initializer)
            {
                stream << " = " << field->initializer;
            }

            stream << std::endl;
        }

        --indent;

        stream << indent << "end structure";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream,
                              VariableDeclarationStatementPtr const& statement)
    {
        stream << "var " << statement->name.text
               << " as " << statement->type_name.text
               << " = " << statement->initializer;

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, AssignmentStatementPtr const& statement)
    {
        stream << statement->name.text << " = " << statement->value;

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, SubCallStatementPtr const& statement)
    {
        stream << statement->name.text << "(" << statement->parameters << ")";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, VariableDeclarationList const& list)
    {
        auto size = list.size();

        for (auto [ i, iter ] = std::pair(0, list.begin()); i < size; ++i, ++iter)
        {
            auto const& item = *iter;

            stream << item->name.text << " as " << item->type_name.text;

            if (item->initializer)
            {
                stream << " = " << item->initializer;
            }

            if (i < (size - 1))
            {
                stream << ", ";
            }
        }

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, Statement const& statement)
    {
        std::visit(StatementHandlers
            {
                .assignment_statement            = [&](auto real) { stream << real; },
                .do_statement                    = [&](auto real) { stream << real; },
                .for_statement                   = [&](auto real) { stream << real; },
                .function_declaration_statement  = [&](auto real) { stream << real; },
                .if_statement                    = [&](auto real) { stream << real; },
                .load_statement                  = [&](auto real) { stream << real; },
                .loop_statement                  = [&](auto real) { stream << real; },
                .select_statement                = [&](auto real) { stream << real; },
                .structure_declaration_statement = [&](auto real) { stream << real; },
                .sub_call_statement              = [&](auto real) { stream << real; },
                .sub_declaration_statement       = [&](auto real) { stream << real; },
                .variable_declaration_statement  = [&](auto real) { stream << real; }
            },
            statement);

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, StatementList const& statement)
    {
        for (auto next : statement)
        {
            stream << indent << next << std::endl;
        }

        return stream;
    }


}
