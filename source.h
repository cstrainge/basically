
#pragma once


namespace basically::source
{


    struct Location
    {
        size_t line = 1;
        size_t column = 1;

        std::fs::path path = "";

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
            Buffer(std::string const& new_text, std::fs::path const& path = "");
            Buffer(std::istream& stream, std::fs::path const& path = "");
            Buffer(std::fs::path const& path);
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

        private:
            std::string read_from(std::istream& stream) const;
            std::string read_from(std::fs::path const& path) const;
    };


}
