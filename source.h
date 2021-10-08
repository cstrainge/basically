
#pragma once


namespace source
{


    struct Location
    {
        size_t line = 1;
        size_t column = 1;

        void next() noexcept;
        void next_line() noexcept;
    };


    std::ostream& operator <<(std::ostream& stream, Location const& location);


    class Buffer
    {
        private:
            std::string text;
            size_t index;
            Location location;

        public:
            Buffer(std::string const& new_text);
            Buffer(std::istream& stream);
            Buffer(Buffer const& buffer) = default;
            Buffer(Buffer&& buffer) = default;
            ~Buffer() = default;

        public:
            Buffer& operator =(Buffer const& buffer) = default;
            Buffer& operator =(Buffer&& buffer) = default;

            operator bool() const noexcept;

        public:
            OptionalChar peek_next(size_t lookahead = 0) const noexcept;
            OptionalChar next() noexcept;
            Location const& current_location() const noexcept;
    };


}
