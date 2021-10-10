
#include "basicly.h"


namespace parse
{


    namespace
    {


        enum class Precedence : size_t
        {
            None = 0,

            Conditional = 1,
            Equality = 2,
            Sum = 3,
            Product = 4
        };


        bool operator <(Precedence lhs, Precedence rhs)
        {
            return static_cast<size_t>(lhs) < static_cast<size_t>(rhs);
        }


        ast::Expression parse_expression(token::Buffer& buffer,
                                         Precedence precedence = Precedence::None);

        ast::ExpressionList parse_parameter_expressions(token::Buffer& buffer);

        ast::Statement parse_statement(token::Buffer& buffer);


        template <typename ItemType>
        std::ostream& operator <<(std::ostream& stream,
                                  std::unordered_map<token::Type, ItemType> const& value)
        {
            token::TypeSet types;

            std::for_each(value.begin(), value.end(), [&](auto next) { types.insert(next.first); });
            stream << types;

            return stream;
        }


        using PrefixHandler = std::function<ast::Expression(token::Buffer&, token::Token const&)>;
        using PrefixHandlerMap = std::unordered_map<token::Type, PrefixHandler>;

        using InfixHandler = std::function<ast::Expression(token::Buffer&, ast::Expression&, token::Token const&)>;
        using InfixHandlerMap = std::unordered_map<token::Type, Precedence>;


        using StatementHandler = std::function<ast::Statement(token::Buffer&, token::Token&)>;
        using StatementHandlerMap = std::unordered_map<token::Type, StatementHandler>;


        [[noreturn]]
        void parse_exception(std::string const& message, source::Location const& location)
        {
            std::stringstream message_stream;

            message_stream << "Error " << location << ": " << message;
            throw std::runtime_error(message_stream.str());
        }


        template <typename ExpectedType>
        [[noreturn]]
        void expected_token_exception(ExpectedType expected, token::Token const& found)
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


        token::Token expect_token(token::Buffer& buffer, token::Type expected)
        {
            auto next = buffer.next();

            if (next.type != expected)
            {
                expected_token_exception(expected, next);
            }

            return next;
        }


        template <token::Type... token_types>
        token::Token expect_one_of(token::Buffer& buffer)
        {
            static const token::TypeSet expected_types({token_types...});

            auto found = buffer.next();
            auto iterator = expected_types.find(found.type);

            if (iterator == expected_types.end())
            {
                expected_token_exception(expected_types, found);
            }

            return found;
        }


        token::Token expect_identifier(token::Buffer& buffer)
        {
            return expect_token(buffer, token::Type::Identifier);
        }


        token::Token expect_numeric_literal(token::Buffer& buffer)
        {
            return expect_one_of<token::Type::LiteralInt, token::Type::LiteralFloat>(buffer);
        }


        void expect_as(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::KeywordAs);
        }


        void expect_assign(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::SymbolAssign);
        }


        void expect_to(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::KeywordTo);
        }


        void expect_open_bracket(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::SymbolOpenBracket);
        }


        void expect_close_bracket(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::SymbolCloseBracket);
        }


        void expect_close_square_bracket(token::Buffer& buffer)
        {
            expect_token(buffer, token::Type::SymbolCloseSquare);
        }


        void expect_end_for(token::Buffer& buffer, token::Token const& start_token)
        {
            expect_token(buffer, token::Type::KeywordEnd);
            expect_token(buffer, start_token.type);
        }


        token::OptionalToken optional_token(token::Buffer& buffer, token::Type type)
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


        bool found_optional_token(token::Buffer& buffer, token::Type type)
        {
            return optional_token(buffer, type).has_value();
        }


        bool found_equal(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::SymbolEqual);
        }


        bool found_while(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::KeywordWhile);
        }


        bool found_step(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::KeywordStep);
        }


        bool found_until(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::KeywordUntil);
        }


        bool found_else(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::KeywordElse);
        }


        bool found_case(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::KeywordCase);
        }


        bool found_open_square_bracket(token::Buffer& buffer)
        {
            return found_optional_token(buffer, token::Type::SymbolOpenSquare);
        }


        ast::Expression parse_literal_expression(token::Buffer& buffer, token::Token const& literal)
        {
            return std::make_unique<ast::LiteralExpression>(literal);
        }


        ast::Expression parse_call_expression(token::Buffer& buffer, token::Token const& name)
        {
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_expressions(buffer);
            expect_close_bracket(buffer);

            return std::make_unique<ast::FunctionCallExpression>(name, std::move(parameters));
        }


        ast::Expression parse_name_expression(token::Buffer& buffer, token::Token const& name)
        {
            if (buffer.peek_next().type == token::Type::SymbolOpenBracket)
            {
                return parse_call_expression(buffer, name);
            }

            ast::Expression subscript;

            if (found_open_square_bracket(buffer))
            {
                subscript = parse_expression(buffer);
                expect_close_square_bracket(buffer);
            }

            return std::make_unique<ast::VariableReadExpression>(name, std::move(subscript));
        }


        ast::Expression parse_group_expression(token::Buffer& buffer, token::Token const& open)
        {
            auto expression = parse_expression(buffer);
            expect_close_bracket(buffer);

            return expression;
        }


        ast::Expression parse_binary_expression(token::Buffer& buffer,
                                                Precedence precedence,
                                                ast::Expression&& left,
                                                token::Token const& operator_token)
        {
            return std::make_unique<ast::BinaryExpression>(operator_token,
                                                           std::move(left),
                                                           parse_expression(buffer, precedence));
        }


        ast::Expression parse_expression(token::Buffer& buffer, Precedence precedence)
        {
            static const PrefixHandlerMap prefix_handlers =
                {
                    { token::Type::LiteralFloat,      parse_literal_expression },
                    { token::Type::LiteralInt,        parse_literal_expression },
                    { token::Type::LiteralString,     parse_literal_expression },
                    { token::Type::Identifier,        parse_name_expression    },
                    { token::Type::SymbolOpenBracket, parse_group_expression   }
                };

            static const InfixHandlerMap infix_handlers =
                {
                    { token::Type::SymbolPlus,        Precedence::Sum },
                    { token::Type::SymbolMinus,       Precedence::Sum },
                    { token::Type::SymbolTimes,       Precedence::Product },
                    { token::Type::SymbolDivide,      Precedence::Product },

                    { token::Type::SymbolEqual,       Precedence::Equality },
                    { token::Type::SymbolNotEqual,    Precedence::Equality },
                    { token::Type::SymbolGreaterThan, Precedence::Equality },
                    { token::Type::SymbolLessThan,    Precedence::Equality },

                    { token::Type::KeywordNot,        Precedence::Conditional },
                    { token::Type::KeywordAnd,        Precedence::Conditional },
                    { token::Type::KeywordOr,         Precedence::Conditional }
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
                                                          std::move(left_expression),
                                                          next);

                next_precedence = peek_next_precedence();
            }

            return left_expression;
        }


        ast::ExpressionList parse_parameter_expressions(token::Buffer& buffer)
        {
            return {};
        }


        ast::StatementBlock parse_block_body_for(token::Buffer& buffer,
                                                 token::Token const& start_token)
        {
            auto not_at_end = [&]() -> bool
                {
                    auto next = buffer.peek_next();

                    return    (next.type != token::Type::KeywordEnd)
                           && (next.type != token::Type::Eof);
                };

            ast::StatementBlock body_statements;

            while (not_at_end())
            {
                body_statements.push_back(parse_statement(buffer));
            }

            expect_end_for(buffer, start_token);

            return body_statements;
        }


        ast::VaraibleDeclarationStatementPtr parse_variable_declaration(token::Buffer& buffer,
                                                                        token::Token& start_token)
        {
            auto name_token = start_token.type != token::Type::Identifier
                                                  ? expect_identifier(buffer)
                                                  : start_token;
            expect_as(buffer);
            auto type_token = expect_identifier(buffer);
            auto value = found_equal(buffer) ? parse_expression(buffer) : nullptr;

            return std::make_unique<ast::VaraibleDeclarationStatement>(start_token.location,
                                                                    name_token,
                                                                    type_token,
                                                                    std::move(value));
        }


        ast::VaraibleDeclarationList parse_parameter_declarations(
                                            token::Buffer& buffer,
                                            token::Type const& delimiter = token::Type::SymbolComma,
                                            token::Type const& end_token = token::Type::None)
        {
            using CheckFunction = std::function<bool(token::Buffer&)>;

            CheckFunction delimiter_check = [&](token::Buffer& buffer) -> bool
                {
                    return found_optional_token(buffer, delimiter);
                };

            CheckFunction end_check = [&](token::Buffer& buffer) -> bool
                {
                    return !found_optional_token(buffer, end_token);
                };

            auto loop_should_continue = (delimiter != token::Type::None) ? delimiter_check
                                                                         : end_check;

            ast::VaraibleDeclarationList vars;

            while (loop_should_continue(buffer))
            {
                if (buffer.peek_next().type == token::Type::Identifier)
                {
                    auto identifier = expect_identifier(buffer);
                    vars.push_back(parse_variable_declaration(buffer, identifier));
                }
            }

            return vars;
        }


        ast::Statement parse_do_statement(token::Buffer& buffer, token::Token& do_token)
        {
            auto terminator = expect_one_of<token::Type::KeywordWhile,
                                            token::Type::KeywordUntil>(buffer);

            auto loop_test = parse_expression(buffer);
            auto loop_body = parse_block_body_for(buffer, do_token);

            return std::make_unique<ast::DoStatement>(do_token.location,
                                                      terminator,
                                                      std::move(loop_test),
                                                      std::move(loop_body));
        }


        ast::Statement parse_for_statement(token::Buffer& buffer, token::Token& for_token)
        {
            auto index_name = expect_identifier(buffer);
            expect_assign(buffer);
            auto start_index = parse_expression(buffer);
            expect_to(buffer);
            auto end_index = parse_expression(buffer);
            auto step_value = found_step(buffer) ? parse_expression(buffer) : nullptr;
            auto loop_body = parse_block_body_for(buffer, for_token);

            return std::make_unique<ast::ForStatement>(for_token.location,
                                                       index_name,
                                                       std::move(start_index),
                                                       std::move(end_index),
                                                       std::move(step_value),
                                                       std::move(loop_body));
        }


        ast::Statement parse_sub_statement(token::Buffer& buffer, token::Token& sub_token)
        {
            auto name = expect_identifier(buffer);
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_declarations(buffer);
            expect_close_bracket(buffer);
            auto sub_body = parse_block_body_for(buffer, sub_token);

            return std::make_unique<ast::SubDeclarationStatement>(sub_token.location,
                                                                  name,
                                                                  std::move(parameters),
                                                                  std::move(sub_body));
        }


        ast::Statement parse_function_statement(token::Buffer& buffer, token::Token& function_token)
        {
            auto name = expect_identifier(buffer);
            expect_open_bracket(buffer);
            auto parameters = parse_parameter_declarations(buffer);
            expect_close_bracket(buffer);
            expect_as(buffer);
            auto return_type = expect_identifier(buffer);
            auto function_body = parse_block_body_for(buffer, function_token);

            return std::make_unique<ast::FunctionDeclarationStatement>(function_token.location,
                                                                       name,
                                                                       std::move(parameters),
                                                                       return_type,
                                                                       std::move(function_body));
        }


        ast::Statement parse_if_statement(token::Buffer& buffer, token::Token& if_token)
        {
            auto expect_then = [&]() { expect_token(buffer, token::Type::KeywordThen); };

            auto found_end_if_tokens = [&]() -> bool
                {
                    auto next = buffer.peek_next();

                    return   (next.type == token::Type::Eof)
                          || (next.type == token::Type::KeywordEnd)
                          || (next.type == token::Type::KeywordElse);
                };

            auto got_optional_else_if_tokens = [&]() -> bool
                {
                    buffer.mark_lookahead();

                    auto else_token = buffer.next();
                    auto if_token = buffer.next();

                    if (   (else_token.type == token::Type::KeywordElse)
                        && (if_token.type == token::Type::KeywordIf))
                    {
                        buffer.commit_lookahead();
                        return true;
                    }

                    buffer.cancel_lookahead();
                    return false;
                };

            auto parse_if_block_statements = [&]() -> ast::StatementBlock
                {
                    ast::StatementBlock statements;

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

                    return std::make_tuple(std::move(test), std::move(body));
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

            auto parse_else_block = [&]() -> ast::StatementBlock
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

            return std::make_unique<ast::IfStatement>(if_token.location,
                                                      std::move(if_head),
                                                      std::move(else_if_blocks),
                                                      std::move(else_block));
        }


        ast::Statement parse_load_statement(token::Buffer& buffer, token::Token& load_token)
        {
            return std::make_unique<ast::LoadStatement>(load_token.location,
                                                        expect_identifier(buffer));
        }


        ast::Statement parse_loop_statement(token::Buffer& buffer, token::Token& loop_token)
        {
            return std::make_unique<ast::LoopStatement>(loop_token.location,
                                                        parse_block_body_for(buffer, loop_token));
        }


        ast::Statement parse_select_statement(token::Buffer& buffer, token::Token& select_token)
        {
            auto found_case_end_tokens = [](token::Buffer& buffer) -> bool
                {
                    auto next = buffer.peek_next();

                    return   (next.type == token::Type::Eof)
                          || (next.type == token::Type::KeywordEnd)
                          || (next.type == token::Type::KeywordElse)
                          || (next.type == token::Type::KeywordCase);
                };

            auto parse_case_block_statements = [&](token::Buffer& buffer) -> ast::StatementBlock
                {
                    ast::StatementBlock statements;

                    while (!found_case_end_tokens(buffer))
                    {
                        statements.push_back(parse_statement(buffer));
                    }

                    return statements;
                };

            auto parse_select_blocks = [&](token::Buffer& buffer) -> ast::ConditionalBlockList
                {
                    ast::ConditionalBlockList block_list;

                    while (found_case(buffer))
                    {
                        auto test = parse_expression(buffer);
                        auto body = parse_case_block_statements(buffer);

                        block_list.push_back(std::make_tuple(std::move(test), std::move(body)));
                    }

                    return block_list;
                };

            auto parse_default_block = [&](token::Buffer& buffer) -> ast::StatementBlock
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

            return std::make_unique<ast::SelectStatement>(select_token.location,
                                                        std::move(text_expression),
                                                        std::move(blocks),
                                                        std::move(default_block));
        }


        ast::Statement parse_structure_statement(token::Buffer& buffer,
                                                 token::Token& structure_token)
        {
            auto name = expect_identifier(buffer);
            auto members = parse_parameter_declarations(buffer,
                                                        token::Type::None,
                                                        token::Type::KeywordEnd);

            expect_token(buffer, structure_token.type);

            return std::make_unique<ast::StructureDeclarationStatement>(structure_token.location,
                                                                        name,
                                                                        std::move(members));
        }


        ast::Statement parse_variable_declaration_statement(token::Buffer& buffer,
                                                            token::Token& var_token)
        {
            return parse_variable_declaration(buffer, var_token);
        }


        ast::Statement parse_identifier_statement(token::Buffer& buffer,
                                                  token::Token& identifier_token)
        {
            // static const StatementHandlerMap statement_parse_map =
            //     {
            //         { token::Type::SymbolAssign,      parse_assignment_statement },
            //         { token::Type::SymbolOpenBracket, parse_sub_call_statement   }
            //     };

            auto next = expect_one_of<token::Type::SymbolOpenBracket,
                                      token::Type::SymbolAssign>(buffer);

            auto parse_assignment_statement = [&]() -> ast::Statement
                {
                    auto value = parse_expression(buffer);
                    return std::make_unique<ast::AssignmentStatement>(next.location,
                                                                      identifier_token,
                                                                      std::move(value));
                };

            auto parse_sub_call_statement = [&]() -> ast::Statement
                {
                    auto parameters = parse_parameter_expressions(buffer);
                    expect_close_bracket(buffer);

                    return std::make_unique<ast::SubCallStatement>(identifier_token.location,
                                                                   identifier_token,
                                                                   std::move(parameters));
                };

            if (next.type == token::Type::SymbolAssign)
            {
                return parse_assignment_statement();
            }

            return parse_sub_call_statement();
        }


        ast::Statement parse_statement(token::Buffer& buffer)
        {
            static const StatementHandlerMap statement_parse_map =
                {
                    { token::Type::KeywordDo,        parse_do_statement                   },
                    { token::Type::KeywordFor,       parse_for_statement                  },
                    { token::Type::KeywordSub,       parse_sub_statement                  },
                    { token::Type::KeywordFunction,  parse_function_statement             },
                    { token::Type::KeywordIf,        parse_if_statement                   },
                    { token::Type::KeywordLoad,      parse_load_statement                 },
                    { token::Type::KeywordLoop,      parse_loop_statement                 },
                    { token::Type::KeywordSelect,    parse_select_statement               },
                    { token::Type::KeywordStructure, parse_structure_statement            },
                    { token::Type::KeywordVar,       parse_variable_declaration_statement },
                    { token::Type::Identifier,       parse_identifier_statement           }
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


    ast::StatementBlock parse_to_ast(token::Buffer& buffer)
    {
        ast::StatementBlock toplevel;

        while (buffer.peek_next().type != token::Type::Eof)
        {
            toplevel.push_back(parse_statement(buffer));
        }

        return toplevel;
    }


}
