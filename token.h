
#pragma once


namespace token
{


    enum class Type
    {
        None, Eof,

        KeywordAnd, KeywordAs, KeywordCase, KeywordDo, KeywordElse, KeywordEnd, KeywordFor,
        KeywordFunction, KeywordIf, KeywordLoad, KeywordLoop, KeywordNext, KeywordNot,
        KeywordOr, KeywordSelect, KeywordStep, KeywordStructure, KeywordSub, KeywordThen, KeywordTo,
        KeywordUntil, KeywordVar, KeywordWhile,

        SymbolOpenBracket, SymbolCloseBracket, SymbolOpenSquare, SymbolCloseSquare, SymbolAssign,
        SymbolEqual, SymbolPlus, SymbolMinus, SymbolTimes, SymbolDivide, SymbolLessThan,
        SymbolGreaterThan, SymbolNotEqual, SymbolComma, SymbolDot, SymbolQuote,

        Identifier,

        LiteralFloat, LiteralInt, LiteralString
    };


    using TypeSet = std::unordered_set<Type>;


    std::ostream& operator <<(std::ostream& stream, Type type);
    std::ostream& operator <<(std::ostream& stream, TypeSet types);


    struct Token
    {
        Type type = Type::None;
        std::string text;
        source::Location location;
    };


    using OptionalToken = std::optional<Token>;


    std::ostream& operator <<(std::ostream& stream, Token const& token);
    std::ostream& operator <<(std::ostream& stream, OptionalToken const& optional_token);


    std::strong_ordering operator <=>(Token const& rhs, Token const& lhs) noexcept;
    std::strong_ordering operator <=>(Type const& rhs, Token const& lhs) noexcept;
    std::strong_ordering operator <=>(Token const& rhs, Type const& lhs) noexcept;

    bool operator ==(Type const& rhs, Token const& lhs) noexcept;
    bool operator ==(Token const& rhs, Type const& lhs) noexcept;


    class Buffer
    {
        private:
            source::Buffer source_buffer;
            std::vector<Token> lookahead_buffer;
            std::list<size_t> marker_stack;

        public:
            Buffer(source::Buffer const& new_buffer);
            Buffer(Buffer const& buffer) = default;
            Buffer(Buffer&& buffer) = default;
            ~Buffer() = default;

        public:
            Buffer& operator =(Buffer const& buffer) = default;
            Buffer& operator =(Buffer&& buffer) = default;

        public:
            void mark_lookahead();
            void commit_lookahead();
            void cancel_lookahead();

        public:
            Token peek_next();
            Token next();

        private:
            bool is_in_lookahead() const noexcept;
            Token extract_next_token() noexcept;

        private:
            Token read_string_token() noexcept;
            Token read_number_token() noexcept;
            Token read_symbol_token() noexcept;
            Token read_identifier_token() noexcept;
            Type read_double_token(Type current, Type expected, Type new_type) noexcept;

            void skip_comment() noexcept;
            void skip_whitespace() noexcept;

            bool is_string_char(OptionalChar const& next) const noexcept;
            bool is_number_start(OptionalChar const& next) const noexcept;
            bool is_number_char(char the_char) const noexcept;
            bool is_symbol_char(OptionalChar const& next) const noexcept;
            bool is_identifier_start(OptionalChar const& next) const noexcept;
            bool is_between(char next, char start, char end) const noexcept;
            bool is_delimiter(OptionalChar const& next) const noexcept;
            bool is_symbol(OptionalChar const& next) const noexcept;
            bool is_comment(OptionalChar const& next) const noexcept;
            bool is_whitespace(OptionalChar const& next) const noexcept;

            template <typename CollectionType>
            bool is_char_in_collection(OptionalChar const& next,
                                       CollectionType const& collection) const noexcept;
    };



}
