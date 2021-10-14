
#include "basically.h"



namespace basically::token
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



    Buffer::Buffer(source::Buffer const& new_buffer)
    : source_buffer(new_buffer),
        lookahead_buffer(),
        marker_stack()
    {
    }


    void Buffer::mark_lookahead()
    {
        auto current_index = is_in_lookahead() ? marker_stack.front() : 0;

        marker_stack.push_front(current_index);
    }


    void Buffer::commit_lookahead()
    {
        if (!is_in_lookahead())
        {
            return;
        }

        if (marker_stack.size() > 1)
        {
            auto current_index = marker_stack.front();
            marker_stack.pop_front();

            marker_stack.front() = current_index;
        }
        else
        {
            marker_stack.clear();
            lookahead_buffer.clear();
        }
    }


    void Buffer::cancel_lookahead()
    {
        if (!is_in_lookahead())
        {
            return;
        }

        marker_stack.pop_front();
    }


    Token Buffer::peek_next()
    {
        mark_lookahead();
        auto next_token = next();
        cancel_lookahead();

        return next_token;
    }


    Token Buffer::next()
    {
        if (!is_in_lookahead())
        {
            return extract_next_token();
        }

        auto token_index = marker_stack.front();
        Token next_token;

        if (token_index >= lookahead_buffer.size())
        {
            next_token = extract_next_token();
            lookahead_buffer.push_back(next_token);
        }
        else
        {
            next_token = lookahead_buffer[token_index];
        }

        marker_stack.front() = token_index + 1;

        return next_token;
    }


    bool Buffer::is_in_lookahead() const noexcept
    {
        return !marker_stack.empty();
    }


    Token Buffer::extract_next_token() noexcept
    {
        // Is the lookahead buffer empty?  If not, don't bother trying to build a new token  the
        // source text, just consume the first entry from the buffer.
        //
        // Note though...  If we are in lookahead mode, then this function is called to build up the
        // lookahead buffer.  So in that case, do not consume from the lookahead buffer, but from
        // the source text.
        if (   (!is_in_lookahead())
            && (!lookahead_buffer.empty()))
        {
            auto next = lookahead_buffer.front();
            lookahead_buffer.erase(lookahead_buffer.begin());

            return next;
        }

        // Looks like we've exhausted the lookahead buffer.  Try to extract a new token from the
        // source text.
        skip_whitespace();

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
            next_token = read_identifier_token();
        }
        else if (is_string_char(next))
        {
            next_token = read_string_token();
        }
        else if (is_number_start(next))
        {
            next_token = read_number_token();
        }
        else if (is_symbol_char(next))
        {
            next_token = read_symbol_token();
        }

        return next_token;
    }


    Token Buffer::read_string_token() noexcept
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


    Token Buffer::read_number_token() noexcept
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


    Token Buffer::read_symbol_token() noexcept
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
                type = read_double_token(type, Type::SymbolAssign, Type::SymbolEqual);
                break;

            case Type::SymbolLessThan:
                type = read_double_token(type, Type::SymbolGreaterThan, Type::SymbolNotEqual);
                break;
        }

        return
            {
                .type = type,
                .text = "",
                .location = location
            };
    }


    Token Buffer::read_identifier_token() noexcept
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


    Type Buffer::read_double_token(Type current, Type expected, Type new_type) noexcept
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


    void Buffer::skip_comment() noexcept
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


    void Buffer::skip_whitespace() noexcept
    {
        while (   is_whitespace(source_buffer.peek_next())
               || is_comment(source_buffer.peek_next()))
        {
            auto next = source_buffer.next();
            if (is_comment(next))
            {
                skip_comment();
            }
        }
    }


    bool Buffer::is_string_char(OptionalChar const& next) const noexcept
    {
        return next && next.value() == '"';
    }


    bool Buffer::is_number_start(OptionalChar const& next) const noexcept
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


    bool Buffer::is_number_char(char the_char) const noexcept
    {
        return is_between(the_char, '0', '9');
    }


    bool Buffer::is_symbol_char(OptionalChar const& next) const noexcept
    {
        return is_char_in_collection(next, symbols);
    }


    bool Buffer::is_identifier_start(OptionalChar const& next) const noexcept
    {
        if (!next)
        {
            return false;
        }

        auto the_char = next.value();

        return    is_between(the_char, 'a', 'z')
               || is_between(the_char, 'A', 'Z')
               || the_char == '_';
    }


    bool Buffer::is_between(char next, char start, char end) const noexcept
    {
        return (next >= start) && (next <= end);
    }


    bool Buffer::is_delimiter(OptionalChar const& next) const noexcept
    {
        return    is_whitespace(next)
               || is_comment(next)
               || is_symbol(next);
    }


    bool Buffer::is_symbol(OptionalChar const& next) const noexcept
    {
        return is_char_in_collection(next, symbols);
    }


    bool Buffer::is_comment(OptionalChar const& next) const noexcept
    {
        return next.has_value() && next.value() == '#';
    }


    bool Buffer::is_whitespace(OptionalChar const& next) const noexcept
    {
        return is_char_in_collection(next, whitespace);
    }


    template <typename CollectionType>
    bool Buffer::is_char_in_collection(OptionalChar const& next,
                                       CollectionType const& collection) const noexcept
    {
        if (!next)
        {
            return false;
        }

        return collection.find(next.value()) != collection.end();
    }


}
