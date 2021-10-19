
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
            if (the_indent.amount > 0)
            {
                stream << std::string(the_indent.amount, ' ');
            }

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
        stream << expression->name.text << "(";

        auto size = expression->parameters.size();

        for (size_t i = 0; i < size; ++i)
        {
            stream << expression->parameters[i];

            if (i < size - 1)
            {
                stream << ", ";
            }
        }

        stream << ")";

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


    std::ostream& operator <<(std::ostream& stream, DoStatementPtr const& statement)
    {
        auto terminator_to_string = [&]()
            {
                auto type = statement->terminator.type;

                assert(   (type == lexing::Type::KeywordWhile)
                       || (type == lexing::Type::KeywordUntil));

                return type == lexing::Type::KeywordWhile ? "while" : "until";
            };

        //++indent;

        stream << "do " << terminator_to_string() << " " << statement->test << std::endl
               << indent << statement->body
               << indent << "end do";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, ForStatementPtr const& statement)
    {
        stream << "ForStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, SubDeclarationStatementPtr const& statement)
    {
        stream << "SubDeclarationStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, FunctionDeclarationStatementPtr const& statement)
    {
        stream << "FunctionDeclarationStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, IfStatementPtr const& statement)
    {
        stream << "IfStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, LoadStatementPtr const& statement)
    {
        stream << "LoadStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, LoopStatementPtr const& statement)
    {
        stream << "LoopStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, SelectStatementPtr const& statement)
    {
        stream << "SelectStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, StructureDeclarationStatementPtr const& statement)
    {
        stream << "StructureDeclarationStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, VariableDeclarationStatementPtr const& statement)
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
        stream << "SubCallStatementPtr";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, VariableDeclarationList const& statement)
    {
        stream << "VariableDeclarationList";

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, Statement const& statement)
    {
        std::visit(StatementHandlers
            {
                .do_statement                    = [&](auto real) { stream << real; },
                .for_statement                   = [&](auto real) { stream << real; },
                .sub_declaration_statement       = [&](auto real) { stream << real; },
                .function_declaration_statement  = [&](auto real) { stream << real; },
                .if_statement                    = [&](auto real) { stream << real; },
                .load_statement                  = [&](auto real) { stream << real; },
                .loop_statement                  = [&](auto real) { stream << real; },
                .select_statement                = [&](auto real) { stream << real; },
                .structure_declaration_statement = [&](auto real) { stream << real; },
                .variable_declaration_statement  = [&](auto real) { stream << real; },
                .assignment_statement            = [&](auto real) { stream << real; },
                .sub_call_statement              = [&](auto real) { stream << real; }
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
