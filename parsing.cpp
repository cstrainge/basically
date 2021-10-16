
#include "basically.h"


namespace basically::parsing
{


    namespace
    {


        enum class Precedence : unsigned int
        {
            None = 0,

            Conditional = 1,
            Equality = 2,
            Sum = 3,
            Product = 4
        };


        bool operator <(Precedence lhs, Precedence rhs)
        {
            return static_cast<unsigned int>(lhs) < static_cast<unsigned int>(rhs);
        }


        ast::Expression parse_expression(lexing::Buffer& buffer,
                                         Precedence precedence = Precedence::None);

        ast::ExpressionList parse_parameter_expressions(lexing::Buffer& buffer);

        ast::Statement parse_statement(lexing::Buffer& buffer);


        template <typename ItemType>
        std::ostream& operator <<(std::ostream& stream,
                                  std::unordered_map<lexing::Type, ItemType> const& value)
        {
            lexing::TypeSet types;

            std::for_each(value.begin(), value.end(), [&](auto next) { types.insert(next.first); });
            stream << types;

            return stream;
        }


        using PrefixHandler = std::function<ast::Expression(lexing::Buffer&, lexing::Token const&)>;
        using PrefixHandlerMap = std::unordered_map<lexing::Type, PrefixHandler>;

        using InfixHandler = std::function<ast::Expression(lexing::Buffer&, ast::Expression&, lexing::Token const&)>;
        using InfixHandlerMap = std::unordered_map<lexing::Type, Precedence>;


        using StatementHandler = std::function<ast::Statement(lexing::Buffer&, lexing::Token&)>;
        using StatementHandlerMap = std::unordered_map<lexing::Type, StatementHandler>;


        [[noreturn]]
        void parse_exception(std::string const& message, source::Location const& location)
        {
            std::stringstream message_stream;

            message_stream << "Error in " << location << ": " << message;
            throw std::runtime_error(message_stream.str());
        }


        template <typename ExpectedType>
        [[noreturn]]
        void expected_token_exception(ExpectedType expected, lexing::Token const& found)
        {
            std::stringstream message_stream;

            message_stream << "Expected " << expected << " but found " << found.type;

            if (!found.text.empty())
            {
                message_stream << ", \"" << found.text << ",\"";
            }

            message_stream << " instead.";

            parse_exception(message_stream.str(), found.location);
        }


        lexing::Token expect_token(lexing::Buffer& buffer, lexing::Type expected)
        {
            auto next = buffer.next();

            if (next.type != expected)
            {
                expected_token_exception(expected, next);
            }

            return next;
        }


        template <lexing::Type... token_types>
        lexing::Token expect_one_of(lexing::Buffer& buffer)
        {
            static const lexing::TypeSet expected_types({token_types...});

            auto found = buffer.next();
            auto iterator = expected_types.find(found.type);

            if (iterator == expected_types.end())
            {
                expected_token_exception(expected_types, found);
            }

            return found;
        }


        lexing::Token expect_identifier(lexing::Buffer& buffer)
        {
            return expect_token(buffer, lexing::Type::Identifier);
        }


        lexing::Token expect_numeric_literal(lexing::Buffer& buffer)
        {
            return expect_one_of<lexing::Type::LiteralInt, lexing::Type::LiteralFloat>(buffer);
        }


        void expect_as(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::KeywordAs);
        }


        void expect_assign(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::SymbolAssign);
        }


        void expect_to(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::KeywordTo);
        }


        void expect_open_bracket(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::SymbolOpenBracket);
        }


        void expect_close_bracket(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::SymbolCloseBracket);
        }


        void expect_close_square_bracket(lexing::Buffer& buffer)
        {
            expect_token(buffer, lexing::Type::SymbolCloseSquare);
        }


        void expect_end_for(lexing::Buffer& buffer, lexing::Token const& start_token)
        {
            expect_token(buffer, lexing::Type::KeywordEnd);
            expect_token(buffer, start_token.type);
        }


        lexing::OptionalToken optional_token(lexing::Buffer& buffer, lexing::Type type)
        {
            buffer.mark_lookahead();
            auto next = buffer.next();

            if (next.type == type)
            {
                buffer.commit_lookahead();
                return next;
            }

            buffer.cancel_lookahead();
            return {};
        }


        bool found_optional_token(lexing::Buffer& buffer, lexing::Type type)
        {
            return optional_token(buffer, type).has_value();
        }


        bool found_assign(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::SymbolAssign);
        }


        bool found_while(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::KeywordWhile);
        }


        bool found_step(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::KeywordStep);
        }


        bool found_until(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::KeywordUntil);
        }


        bool found_else(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::KeywordElse);
        }


        bool found_case(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::KeywordCase);
        }


        bool found_open_square_bracket(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::SymbolOpenSquare);
        }


        bool found_comma(lexing::Buffer& buffer)
        {
            return found_optional_token(buffer, lexing::Type::SymbolComma);
        }


        ast::Expression parse_literal_expression(lexing::Buffer& buffer,
                                                 lexing::Token const& literal)
        {
            return std::make_shared<ast::LiteralExpression>(literal);
        }


        ast::Expression parse_call_expression(lexing::Buffer& buffer, lexing::Token const& name)
        {
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_expressions(buffer);
            expect_close_bracket(buffer);

            return std::make_shared<ast::FunctionCallExpression>(name, parameters);
        }


        ast::Expression parse_name_expression(lexing::Buffer& buffer, lexing::Token const& name)
        {
            if (buffer.peek_next().type == lexing::Type::SymbolOpenBracket)
            {
                return parse_call_expression(buffer, name);
            }

            ast::Expression subscript;

            if (found_open_square_bracket(buffer))
            {
                subscript = parse_expression(buffer);
                expect_close_square_bracket(buffer);
            }

            return std::make_shared<ast::VariableReadExpression>(name, subscript);
        }


        ast::Expression parse_group_expression(lexing::Buffer& buffer, lexing::Token const& open)
        {
            auto expression = parse_expression(buffer);
            expect_close_bracket(buffer);

            return expression;
        }


        ast::Expression parse_binary_expression(lexing::Buffer& buffer,
                                                Precedence precedence,
                                                ast::Expression const& left,
                                                lexing::Token const& operator_token)
        {
            return std::make_shared<ast::BinaryExpression>(operator_token,
                                                           left,
                                                           parse_expression(buffer, precedence));
        }


        ast::Expression parse_expression(lexing::Buffer& buffer, Precedence precedence)
        {
            static const PrefixHandlerMap prefix_handlers =
                {
                    { lexing::Type::LiteralFloat,      parse_literal_expression },
                    { lexing::Type::LiteralInt,        parse_literal_expression },
                    { lexing::Type::LiteralString,     parse_literal_expression },
                    { lexing::Type::Identifier,        parse_name_expression    },
                    { lexing::Type::SymbolOpenBracket, parse_group_expression   }
                };

            static const InfixHandlerMap infix_handlers =
                {
                    { lexing::Type::SymbolPlus,        Precedence::Sum         },
                    { lexing::Type::SymbolMinus,       Precedence::Sum         },
                    { lexing::Type::SymbolTimes,       Precedence::Product     },
                    { lexing::Type::SymbolDivide,      Precedence::Product     },

                    { lexing::Type::SymbolEqual,       Precedence::Equality    },
                    { lexing::Type::SymbolNotEqual,    Precedence::Equality    },
                    { lexing::Type::SymbolGreaterThan, Precedence::Equality    },
                    { lexing::Type::SymbolLessThan,    Precedence::Equality    },

                    { lexing::Type::KeywordNot,        Precedence::Conditional },
                    { lexing::Type::KeywordAnd,        Precedence::Conditional },
                    { lexing::Type::KeywordOr,         Precedence::Conditional }
                };

            auto peek_next_precedence = [&]() -> Precedence
                {
                    auto infix_iter = infix_handlers.find(buffer.peek_next().type);

                    return infix_iter != infix_handlers.end() ? infix_iter->second
                                                              : Precedence::None;
                };

            auto next = buffer.next();
            auto prefix_iter = prefix_handlers.find(next.type);

            if (prefix_iter == prefix_handlers.end())
            {
                expected_token_exception(prefix_handlers, next);
            }

            auto prefix_handler = prefix_iter->second;
            auto left_expression = prefix_handler(buffer, next);

            auto next_precedence = peek_next_precedence();

            while (precedence < next_precedence)
            {
                next = buffer.next();
                left_expression = parse_binary_expression(buffer,
                                                          next_precedence,
                                                          left_expression,
                                                          next);

                next_precedence = peek_next_precedence();
            }

            return left_expression;
        }


        ast::ExpressionList parse_parameter_expressions(lexing::Buffer& buffer)
        {
            ast::ExpressionList expressions;

            do
            {
                expressions.push_back(parse_expression(buffer));
            }
            while (found_comma(buffer));

            return expressions;
        }


        ast::StatementList parse_block_body_for(lexing::Buffer& buffer,
                                                lexing::Token const& start_token)
        {
            auto not_at_end = [&]() -> bool
                {
                    auto next = buffer.peek_next();

                    return    (next.type != lexing::Type::KeywordEnd)
                           && (next.type != lexing::Type::Eof);
                };

            ast::StatementList body_statements;

            while (not_at_end())
            {
                body_statements.push_back(parse_statement(buffer));
            }

            expect_end_for(buffer, start_token);

            return body_statements;
        }


        ast::VariableDeclarationStatementPtr parse_variable_declaration(
                                                                   lexing::Buffer& buffer,
                                                                   lexing::Token const& start_token)
        {
            auto name_token = start_token.type != lexing::Type::Identifier
                ? expect_identifier(buffer)
                : start_token;

            expect_as(buffer);

            auto type_token = expect_identifier(buffer);
            auto value = found_assign(buffer) ? parse_expression(buffer)
                                              : ast::OptionalExpression();

            return std::make_shared<ast::VariableDeclarationStatement>(start_token.location,
                                                                       name_token,
                                                                       type_token,
                                                                       value);
        }


        ast::VariableDeclarationList parse_parameter_declarations(
                                                 lexing::Buffer& buffer,
                                                 lexing::Type delimiter = lexing::Type::SymbolComma,
                                                 lexing::Type end_token = lexing::Type::None)
        {
            auto not_at_end = [&](lexing::Token const& next) -> bool
                {
                    if (   (delimiter != lexing::Type::None)
                        && (next.type == delimiter))
                    {
                        return true;
                    }

                    return next.type != end_token;
                };

            ast::VariableDeclarationList vars;
            lexing::Token next;

            do
            {
                next = buffer.peek_next();

                if (next != end_token)
                {
                    vars.push_back(parse_variable_declaration(buffer, {}));

                    if (!found_optional_token(buffer, delimiter))
                    {
                        next = { .type = end_token };
                    }
                }
            }
            while (not_at_end(next));

            return vars;
        }


        ast::Statement parse_do_statement(lexing::Buffer& buffer, lexing::Token& do_token)
        {
            auto terminator = expect_one_of<lexing::Type::KeywordWhile,
                                            lexing::Type::KeywordUntil>(buffer);

            auto loop_test = parse_expression(buffer);
            auto loop_body = parse_block_body_for(buffer, do_token);

            return std::make_shared<ast::DoStatement>(do_token.location,
                                                      terminator,
                                                      loop_test,
                                                      loop_body);
        }


        ast::Statement parse_for_statement(lexing::Buffer& buffer, lexing::Token& for_token)
        {
            auto index_name = expect_identifier(buffer);
            expect_assign(buffer);
            auto start_index = parse_expression(buffer);
            expect_to(buffer);
            auto end_index = parse_expression(buffer);
            auto step_value = found_step(buffer) ? parse_expression(buffer)
                                                 : ast::OptionalExpression();
            auto loop_body = parse_block_body_for(buffer, for_token);

            return std::make_shared<ast::ForStatement>(for_token.location,
                                                       index_name,
                                                       start_index,
                                                       end_index,
                                                       step_value,
                                                       loop_body);
        }


        ast::Statement parse_sub_statement(lexing::Buffer& buffer, lexing::Token& sub_token)
        {
            auto name = expect_identifier(buffer);
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_declarations(buffer);
            expect_close_bracket(buffer);
            auto sub_body = parse_block_body_for(buffer, sub_token);

            return std::make_shared<ast::SubDeclarationStatement>(sub_token.location,
                                                                  name,
                                                                  parameters,
                                                                  sub_body);
        }


        ast::Statement parse_function_statement(lexing::Buffer& buffer,
                                                lexing::Token& function_token)
        {
            auto name = expect_identifier(buffer);
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_declarations(buffer);
            expect_close_bracket(buffer);
            expect_as(buffer);
            auto return_type = expect_identifier(buffer);
            auto function_body = parse_block_body_for(buffer, function_token);

            return std::make_shared<ast::FunctionDeclarationStatement>(function_token.location,
                                                                       name,
                                                                       parameters,
                                                                       return_type,
                                                                       function_body);
        }


        ast::Statement parse_if_statement(lexing::Buffer& buffer, lexing::Token& if_token)
        {
            auto expect_then = [&]() { expect_token(buffer, lexing::Type::KeywordThen); };

            auto found_end_if_tokens = [&]() -> bool
                {
                    auto next = buffer.peek_next();

                    return   (next.type == lexing::Type::Eof)
                          || (next.type == lexing::Type::KeywordEnd)
                          || (next.type == lexing::Type::KeywordElse);
                };

            auto got_optional_else_if_tokens = [&]() -> bool
                {
                    buffer.mark_lookahead();

                    auto else_token = buffer.next();
                    auto if_token = buffer.next();

                    if (   (else_token.type == lexing::Type::KeywordElse)
                        && (if_token.type == lexing::Type::KeywordIf))
                    {
                        buffer.commit_lookahead();
                        return true;
                    }

                    buffer.cancel_lookahead();
                    return false;
                };

            auto parse_if_block_statements = [&]() -> ast::StatementList
                {
                    ast::StatementList statements;

                    while (!found_end_if_tokens())
                    {
                        statements.push_back(parse_statement(buffer));
                    }

                    return statements;
                };

            auto parse_if_block = [&]() -> ast::ConditionalBlock
                {
                    auto test = parse_expression(buffer);
                    expect_then();
                    auto body = parse_if_block_statements();

                    return std::make_tuple(test, body);
                };

            auto parse_else_if_blocks = [&]() -> ast::ConditionalBlockList
                {
                    ast::ConditionalBlockList blocks;

                    while (got_optional_else_if_tokens())
                    {
                        blocks.push_back(parse_if_block());
                    }

                    return blocks;
                };

            auto parse_else_block = [&]() -> ast::StatementList
                {
                    if (found_else(buffer))
                    {
                        return parse_if_block_statements();
                    }

                    return {};
                };

            auto if_head = parse_if_block();
            auto else_if_blocks = parse_else_if_blocks();
            auto else_block = parse_else_block();

            expect_end_for(buffer, if_token);

            return std::make_shared<ast::IfStatement>(if_token.location,
                                                      if_head,
                                                      else_if_blocks,
                                                      else_block);
        }


        ast::Statement parse_load_statement(lexing::Buffer& buffer, lexing::Token& load_token)
        {
            return std::make_shared<ast::LoadStatement>(load_token.location,
                                                        expect_identifier(buffer));
        }


        ast::Statement parse_loop_statement(lexing::Buffer& buffer, lexing::Token& loop_token)
        {
            return std::make_shared<ast::LoopStatement>(loop_token.location,
                                                        parse_block_body_for(buffer, loop_token));
        }


        ast::Statement parse_select_statement(lexing::Buffer& buffer, lexing::Token& select_token)
        {
            auto found_case_end_tokens = [](lexing::Buffer& buffer) -> bool
                {
                    auto next = buffer.peek_next();

                    return   (next.type == lexing::Type::Eof)
                          || (next.type == lexing::Type::KeywordEnd)
                          || (next.type == lexing::Type::KeywordElse)
                          || (next.type == lexing::Type::KeywordCase);
                };

            auto parse_case_block_statements = [&](lexing::Buffer& buffer) -> ast::StatementList
                {
                    ast::StatementList statements;

                    while (!found_case_end_tokens(buffer))
                    {
                        statements.push_back(parse_statement(buffer));
                    }

                    return statements;
                };

            auto parse_select_blocks = [&](lexing::Buffer& buffer) -> ast::ConditionalBlockList
                {
                    ast::ConditionalBlockList block_list;

                    while (found_case(buffer))
                    {
                        auto test = parse_expression(buffer);
                        auto body = parse_case_block_statements(buffer);

                        block_list.push_back(std::make_tuple(test, body));
                    }

                    return block_list;
                };

            auto parse_default_block = [&](lexing::Buffer& buffer) -> ast::StatementList
                {
                    if (found_else(buffer))
                    {
                        return parse_case_block_statements(buffer);
                    }

                    return {};
                };

            auto text_expression = parse_expression(buffer);
            auto blocks = parse_select_blocks(buffer);
            auto default_block = parse_default_block(buffer);

            expect_end_for(buffer, select_token);

            return std::make_shared<ast::SelectStatement>(select_token.location,
                                                          text_expression,
                                                          blocks,
                                                          default_block);
        }


        ast::Statement parse_structure_statement(lexing::Buffer& buffer,
                                                 lexing::Token& structure_token)
        {
            auto name = expect_identifier(buffer);
            auto members = parse_parameter_declarations(buffer,
                                                        lexing::Type::None,
                                                        lexing::Type::KeywordEnd);

            expect_token(buffer, structure_token.type);

            return std::make_shared<ast::StructureDeclarationStatement>(structure_token.location,
                                                                        name,
                                                                        members);
        }


        ast::Statement parse_variable_declaration_statement(lexing::Buffer& buffer,
                                                            lexing::Token& var_token)
        {
            return parse_variable_declaration(buffer, var_token);
        }


        ast::Statement parse_identifier_statement(lexing::Buffer& buffer,
                                                  lexing::Token& identifier_token)
        {
            auto next = expect_one_of<lexing::Type::SymbolOpenBracket,
                                      lexing::Type::SymbolAssign>(buffer);

            auto parse_assignment_statement = [&]() -> ast::Statement
                {
                    auto value = parse_expression(buffer);
                    return std::make_shared<ast::AssignmentStatement>(next.location,
                                                                      identifier_token,
                                                                      value);
                };

            auto parse_sub_call_statement = [&]() -> ast::Statement
                {
                    auto parameters = parse_parameter_expressions(buffer);
                    expect_close_bracket(buffer);

                    return std::make_shared<ast::SubCallStatement>(identifier_token.location,
                                                                   identifier_token,
                                                                   parameters);
                };

            if (next.type == lexing::Type::SymbolAssign)
            {
                return parse_assignment_statement();
            }

            return parse_sub_call_statement();
        }


        ast::Statement parse_statement(lexing::Buffer& buffer)
        {
            static const StatementHandlerMap statement_parse_map =
                {
                    { lexing::Type::KeywordDo,        parse_do_statement                   },
                    { lexing::Type::KeywordFor,       parse_for_statement                  },
                    { lexing::Type::KeywordSub,       parse_sub_statement                  },
                    { lexing::Type::KeywordFunction,  parse_function_statement             },
                    { lexing::Type::KeywordIf,        parse_if_statement                   },
                    { lexing::Type::KeywordLoad,      parse_load_statement                 },
                    { lexing::Type::KeywordLoop,      parse_loop_statement                 },
                    { lexing::Type::KeywordSelect,    parse_select_statement               },
                    { lexing::Type::KeywordStructure, parse_structure_statement            },
                    { lexing::Type::KeywordVar,       parse_variable_declaration_statement },
                    { lexing::Type::Identifier,       parse_identifier_statement           }
                };

            auto next = buffer.next();
            auto found_handler = statement_parse_map.find(next.type);

            if (found_handler == statement_parse_map.end())
            {
                expected_token_exception(statement_parse_map, next);
            }

            return found_handler->second(buffer, next);
        }


    }


    ast::StatementList parse_to_ast(lexing::Buffer& buffer)
    {
        ast::StatementList toplevel;

        while (buffer.peek_next().type != lexing::Type::Eof)
        {
            toplevel.push_back(parse_statement(buffer));
        }

        return toplevel;
    }


}
