
#include "basically.h"



namespace basically::source
{


    void Location::next() noexcept
    {
        ++column;
    }


    void Location::next_line() noexcept
    {
        ++line;
        column = 1;
    }


    std::ostream& operator <<(std::ostream& stream, Location const& location)
    {
        stream << location.path.string() << "(" << location.line << ", " << location.column << ")";
        return stream;
    }


    Buffer::Buffer(std::string const& new_text, std::fs::path const& new_path)
    : text(new_text),
      index(0),
      location({ .path = new_path })
    {
    }


    Buffer::Buffer(std::istream& stream, std::fs::path const& new_path)
    : Buffer(read_from(stream), new_path)
    {
    }


    Buffer::Buffer(std::fs::path const& new_path)
    : Buffer(read_from(new_path), new_path)
    {
    }


    Buffer::operator bool() const noexcept
    {
        return !text.empty() && index < text.size();
    }


    OptionalChar Buffer::peek_next(size_t lookahead) const noexcept
    {
        auto next_index = index + lookahead;

        if (next_index >= text.size())
        {
            return {};
        }

        return text[next_index];
    }


    OptionalChar Buffer::next() noexcept
    {
        auto next_char = peek_next();

        if (!next_char)
        {
            return next_char;
        }

        ++index;

        if (next_char.value() == '\n')
        {
            location.next_line();
        }
        else
        {
            location.next();
        }

        return next_char;
    }


    Location const& Buffer::current_location() const noexcept
    {
        return location;
    }


    std::string Buffer::read_from(std::istream& stream) const
    {
        auto begin = std::istreambuf_iterator<char>(stream);
        auto end = std::istreambuf_iterator<char>();

        return std::string(begin, end);
    }


    std::string Buffer::read_from(std::fs::path const& path) const
    {
        auto file = std::ifstream(path.string());

        if (!file.is_open())
        {
            throw std::runtime_error("Could not open source file " + path.string() + ".");
        }

        return read_from(file);
    }


}
