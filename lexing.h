
#pragma once


namespace basically::lexing
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
    using TokenList = std::vector<Token>;


    std::ostream& operator <<(std::ostream& stream, Token const& token);
    std::ostream& operator <<(std::ostream& stream, OptionalToken const& optional_token);


    std::strong_ordering operator <=>(Token const& rhs, Token const& lhs) noexcept;
    std::strong_ordering operator <=>(Type const& rhs, Token const& lhs) noexcept;
    std::strong_ordering operator <=>(Token const& rhs, Type const& lhs) noexcept;


    bool operator ==(Type const& rhs, Token const& lhs) noexcept;
    bool operator ==(Token const& rhs, Type const& lhs) noexcept;


    class Buffer;
    std::ostream& operator <<(std::ostream& stream, Buffer const& buffer);


    class Buffer
    {
        public:
            friend std::ostream& operator <<(std::ostream& stream, Buffer const& buffer);

        private:
            TokenList tokens;
            std::list<size_t> index_stack;

        public:
            Buffer();
            Buffer(source::Buffer& source_buffer);
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
    };


}
