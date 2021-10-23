
#include "basically.h"


namespace basically::lexing
{


    namespace
    {


        static const std::unordered_map<Type, std::string> type_string_map =
            {
                { Type::None,               "nothing"                        },
                { Type::Eof,                "the end of the file"            },
                { Type::KeywordAnd,         "the keyword and"                },
                { Type::KeywordAs,          "the keyword as"                 },
                { Type::KeywordCase,        "the keyword case"               },
                { Type::KeywordDo,          "the keyword do"                 },
                { Type::KeywordElse,        "the keyword else"               },
                { Type::KeywordEnd,         "the keyword end"                },
                { Type::KeywordFor,         "the keyword for"                },
                { Type::KeywordFunction,    "the keyword function"           },
                { Type::KeywordIf,          "the keyword if"                 },
                { Type::KeywordLoad,        "the keyword load"               },
                { Type::KeywordLoop,        "the keyword loop"               },
                { Type::KeywordNext,        "the keyword next"               },
                { Type::KeywordNot,         "the keyword not"                },
                { Type::KeywordOr,          "the keyword or"                 },
                { Type::KeywordSelect,      "the keyword select"             },
                { Type::KeywordStep,        "the keyword step"               },
                { Type::KeywordStructure,   "the keyword structure"          },
                { Type::KeywordSub,         "the keyword sub"                },
                { Type::KeywordThen,        "the keyword then"               },
                { Type::KeywordTo,          "the keyword to"                 },
                { Type::KeywordUntil,       "the keyword until"              },
                { Type::KeywordVar,         "the keyword var"                },
                { Type::KeywordWhile,       "the keyword while"              },
                { Type::SymbolOpenBracket,  "the symbol ("                   },
                { Type::SymbolCloseBracket, "the symbol )"                   },
                { Type::SymbolOpenSquare,   "the symbol ["                   },
                { Type::SymbolCloseSquare,  "the symbol ]"                   },
                { Type::SymbolAssign,       "the symbol ="                   },
                { Type::SymbolEqual,        "the symbol =="                  },
                { Type::SymbolPlus,         "the symbol +"                   },
                { Type::SymbolMinus,        "the symbol -"                   },
                { Type::SymbolTimes,        "the symbol *"                   },
                { Type::SymbolDivide,       "the symbol /"                   },
                { Type::SymbolLessThan,     "the symbol <"                   },
                { Type::SymbolGreaterThan,  "the symbol >"                   },
                { Type::SymbolNotEqual,     "the symbol <>"                  },
                { Type::SymbolComma,        "the symbol ,"                   },
                { Type::SymbolDot,          "the symbol ."                   },
                { Type::SymbolQuote,        "the symbol \""                  },
                { Type::Identifier,         "an identifier"                  },
                { Type::LiteralFloat,       "a literal floating point value" },
                { Type::LiteralInt,         "a literal integer value"        },
                { Type::LiteralString,      "a literal string value"         }
            };


        static const std::unordered_map<std::string, Type> keywords =
            {
                { "and",       Type::KeywordAnd       },
                { "as",        Type::KeywordAs        },
                { "case",      Type::KeywordCase      },
                { "do",        Type::KeywordDo        },
                { "else",      Type::KeywordElse      },
                { "end",       Type::KeywordEnd       },
                { "for",       Type::KeywordFor       },
                { "function",  Type::KeywordFunction  },
                { "if",        Type::KeywordIf        },
                { "load",      Type::KeywordLoad      },
                { "loop",      Type::KeywordLoop      },
                { "next",      Type::KeywordNext      },
                { "not",       Type::KeywordNot       },
                { "or",        Type::KeywordOr        },
                { "select",    Type::KeywordSelect    },
                { "step",      Type::KeywordStep      },
                { "structure", Type::KeywordStructure },
                { "sub",       Type::KeywordSub       },
                { "then",      Type::KeywordThen      },
                { "to",        Type::KeywordTo        },
                { "until",     Type::KeywordUntil     },
                { "var",       Type::KeywordVar       },
                { "while",     Type::KeywordWhile     }
            };


        static const std::unordered_map<char, Type> symbols =
            {
                { '(', Type::SymbolOpenBracket  },
                { ')', Type::SymbolCloseBracket },
                { '[', Type::SymbolOpenSquare   },
                { ']', Type::SymbolCloseSquare  },
                { '=', Type::SymbolAssign       },
                { '+', Type::SymbolPlus         },
                { '-', Type::SymbolMinus        },
                { '*', Type::SymbolTimes        },
                { '/', Type::SymbolDivide       },
                { '<', Type::SymbolLessThan     },
                { '>', Type::SymbolGreaterThan  },
                { ',', Type::SymbolComma        },
                { '.', Type::SymbolDot          },
                { '"', Type::SymbolQuote        }
            };


        static const std::unordered_set<char> whitespace =
            {
                ' ', '\t', '\n'
            };


        template <typename CollectionType>
        bool is_char_in_collection(OptionalChar const& next,
                                   CollectionType const& collection) noexcept
        {
            if (!next)
            {
                return false;
            }

            return collection.find(next.value()) != collection.end();
        }


        bool is_char_between(OptionalChar next, char start, char end) noexcept
        {
            if (!next)
            {
                return false;
            }

            auto the_char = next.value();

            return (the_char >= start) && (the_char <= end);
        }


        bool is_whitespace(OptionalChar const& next) noexcept
        {
            return is_char_in_collection(next, whitespace);
        }


        bool is_comment(OptionalChar const& next) noexcept
        {
            return next.has_value() && next.value() == '#';
        }


        bool is_symbol(OptionalChar const& next) noexcept
        {
            return is_char_in_collection(next, symbols);
        }


        bool is_delimiter(OptionalChar const& next) noexcept
        {
            return    is_whitespace(next)
                   || is_comment(next)
                   || is_symbol(next);
        }


        bool is_number_char(OptionalChar const& next) noexcept
        {
            return is_char_between(next, '0', '9');
        }


        bool is_number_start(source::Buffer& source_buffer, OptionalChar const& next) noexcept
        {
            if (!next)
            {
                return false;
            }

            auto the_char = next.value();

            if (is_number_char(the_char))
            {
                return true;
            }

            auto later = source_buffer.peek_next(1);

            if (   ((the_char == '+') || (the_char == '-'))
                && (later && is_number_char(later.value())))
            {
                return true;
            }

            return false;
        }


        bool is_string_char(OptionalChar const& next) noexcept
        {
            return next && next.value() == '"';
        }


        bool is_identifier_start(OptionalChar const& next) noexcept
        {
            if (!next)
            {
                return false;
            }

            auto the_char = next.value();

            return    is_char_between(the_char, 'a', 'z')
                   || is_char_between(the_char, 'A', 'Z')
                   || the_char == '_';
        }


        void skip_comment(source::Buffer& source_buffer) noexcept
        {
            auto next = source_buffer.peek_next();

            if (is_comment(next))
            {
                source_buffer.next();
            }

            do
            {
                next = source_buffer.next();
            }
            while (next && next.value() != '\n');
        }


        void skip_whitespace(source::Buffer& source_buffer) noexcept
        {
            while (   is_whitespace(source_buffer.peek_next())
                   || is_comment(source_buffer.peek_next()))
            {
                auto next = source_buffer.next();
                if (is_comment(next))
                {
                    skip_comment(source_buffer);
                }
            }
        }

        Type read_double_token_type(source::Buffer& source_buffer,
                                    Type current,
                                    Type expected,
                                    Type new_type) noexcept
        {
            auto next = source_buffer.peek_next();

            if (!next)
            {
                return current;
            }

            auto found_symbol = symbols.find(next.value());

            if (found_symbol == symbols.end())
            {
                return current;
            }

            if (found_symbol->second != expected)
            {
                return current;
            }

            source_buffer.next();

            return new_type;
        }



        Token read_symbol_token(source::Buffer& source_buffer) noexcept
        {
            auto location = source_buffer.current_location();
            auto next = source_buffer.next();

            if (!next)
            {
                return {};
            }

            auto found_symbol = symbols.find(next.value());

            if (found_symbol == symbols.end())
            {
                return {};
            }

            auto type = found_symbol->second;

            switch (type)
            {
                case Type::SymbolAssign:
                    type = read_double_token_type(source_buffer,
                                                  type,
                                                  Type::SymbolAssign,
                                                  Type::SymbolEqual);
                    break;

                case Type::SymbolLessThan:
                    type = read_double_token_type(source_buffer,
                                                  type,
                                                  Type::SymbolGreaterThan,
                                                  Type::SymbolNotEqual);
                    break;
            }

            return
                {
                    .type = type,
                    .text = "",
                    .location = location
                };
        }


        Token read_number_token(source::Buffer& source_buffer) noexcept
        {
            auto is_one_of = [](char the_char, std::unordered_set<char> const& options) -> bool
                {
                    return options.find(the_char) != options.end();
                };

            auto next = source_buffer.peek_next();

            if (!next)
            {
                return {};
            }

            std::string number_string;
            auto location = source_buffer.current_location();

            do
            {
                source_buffer.next();
                number_string.push_back(next.value());
                next = source_buffer.peek_next();
            }
            while (next && (   is_number_char(next.value())
                            || is_one_of(next.value(), { '+', '-', 'e', 'E', '.' })));

            return
                {
                    .type = number_string.find('.') != std::string::npos
                            ? Type::LiteralFloat
                            : Type::LiteralInt,
                    .text = number_string,
                    .location = location,
                };
        }


        Token read_string_token(source::Buffer& source_buffer) noexcept
        {
            std::string literal_string;

            auto next = source_buffer.peek_next();
            auto location = source_buffer.current_location();

            if (next && next.value() == '"')
            {
                source_buffer.next();
            }

            do
            {
                next = source_buffer.next();

                if (!is_string_char(next))
                {
                    literal_string.push_back(next.value());
                }

                next = source_buffer.peek_next();
            }
            while (next && !is_string_char(next));

            if (!is_string_char(next))
            {
                return {};
            }

            source_buffer.next();

            return
                {
                    .type = Type::LiteralString,
                    .text = literal_string,
                    .location = location
                };
        }


        Token read_identifier_token(source::Buffer& source_buffer) noexcept
        {
            std::string identifier;

            auto next = source_buffer.peek_next();
            auto location = source_buffer.current_location();

            while (next && !is_delimiter(next))
            {
                source_buffer.next();
                identifier.push_back(next.value());

                next = source_buffer.peek_next();
            }

            if (identifier.empty())
            {
                return {};
            }

            auto keyword_iter = keywords.find(identifier);
            auto type = Type::Identifier;

            if (keyword_iter != keywords.end())
            {
                type = keyword_iter->second;
                identifier.clear();
            }

            return
                {
                    .type = type,
                    .text = identifier,
                    .location = location
                };
        }


        Token extract_next_token(source::Buffer& source_buffer) noexcept
        {
            skip_whitespace(source_buffer);

            auto next = source_buffer.peek_next();

            if (!next)
            {
                return
                    {
                        .type = Type::Eof,
                        .location = source_buffer.current_location()
                    };
            }

            Token next_token;

            if (is_identifier_start(next))
            {
                next_token = read_identifier_token(source_buffer);
            }
            else if (is_string_char(next))
            {
                next_token = read_string_token(source_buffer);
            }
            else if (is_number_start(source_buffer, next))
            {
                next_token = read_number_token(source_buffer);
            }
            else if (is_symbol(next))
            {
                next_token = read_symbol_token(source_buffer);
            }

            return next_token;
        }


        TokenList extract_tokens(source::Buffer& source_buffer)
        {
            TokenList tokens;
            Type next = Type::None;

            do
            {
                auto token = extract_next_token(source_buffer);
                next = token.type;

                tokens.push_back(token);
            }
            while (next != Type::Eof);

            return tokens;
        }


    }


    std::ostream& operator <<(std::ostream& stream, Type type)
    {
        auto map_item = type_string_map.find(type);
        auto string = map_item != type_string_map.end() ? map_item->second
                                                        : "unknown token type";

        stream << string;

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, TypeSet types)
    {
        auto iterator = types.begin();

        while (iterator != types.end())
        {
            auto this_one = *iterator;
            ++iterator;

            stream << this_one;

            switch (std::distance(iterator, types.end()))
            {
                default:
                    stream << ", ";
                    break;

                case 1:
                    stream << " or ";
                    break;

                case 0:
                    break;
            }
        }

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, Token const& token)
    {
        stream << token.location << " " << token.type;

        if (!token.text.empty())
        {
            stream << " " << token.text;
        }

        return stream;
    }


    std::ostream& operator <<(std::ostream& stream, OptionalToken const& optional_token)
    {
        if (optional_token)
        {
            auto token = optional_token.value();

            stream << "[ " << token.location << " " << token.type;

            if (!token.text.empty())
            {
                stream << " " << token.text;
            }

            stream << " ]";
        }
        else
        {
            stream << "[]";
        }

        return stream;
    }


    std::strong_ordering operator <=>(Token const& rhs, Token const& lhs) noexcept
    {
        if (rhs.type != lhs.type)
        {
            return rhs.type <=> lhs.type;
        }

        if (rhs.text != rhs.text)
        {
            return rhs.text <=> lhs.text;
        }

        return std::strong_ordering::equal;
    }


    std::strong_ordering operator <=>(Type const& rhs, Token const& lhs) noexcept
    {
        return rhs <=> lhs.type;
    }


    std::strong_ordering operator <=>(Token const& rhs, Type const& lhs) noexcept
    {
        return rhs.type <=> lhs;
    }


    bool operator ==(Type const& rhs, Token const& lhs) noexcept
    {
        return (rhs <=> lhs) == std::strong_ordering::equal;
    }


    bool operator ==(Token const& rhs, Type const& lhs) noexcept
    {
        return (rhs <=> lhs) == std::strong_ordering::equal;
    }


    std::ostream& operator <<(std::ostream& stream, Buffer const& buffer)
    {
        for (size_t i = 0; i < buffer.tokens.size(); ++i)
        {
            stream << (i == buffer.index_stack.front() ? " --> " : "     ")
                   << buffer.tokens[i] << std::endl;
        }

        return stream;
    }


    Buffer::Buffer()
    : tokens(1, { .type = Type::Eof }),
      index_stack(1, 0)
    {
    }


    Buffer::Buffer(source::Buffer& source_buffer)
    : tokens(extract_tokens(source_buffer)),
      index_stack(1, 0)
    {
        assert(tokens.size() >= 1);
    }


    void Buffer::mark_lookahead()
    {
        index_stack.push_front(index_stack.front());
    }


    void Buffer::commit_lookahead()
    {
        assert(is_in_lookahead());

        auto current_index = index_stack.front();
        index_stack.pop_front();

        index_stack.front() = current_index;
    }


    void Buffer::cancel_lookahead()
    {
        assert(is_in_lookahead());

        index_stack.pop_front();
    }


    Token Buffer::peek_next()
    {
        assert(!index_stack.empty());
        assert(index_stack.front() < tokens.size());

        auto index = index_stack.front();
        return tokens[index];
    }


    Token Buffer::next()
    {
        auto const& next = peek_next();
        index_stack.front() = index_stack.front() + 1;

        return next;
    }


    bool Buffer::is_in_lookahead() const noexcept
    {
        return index_stack.size() > 1;
    }


}
