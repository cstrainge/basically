
#include "basically.h"



namespace source
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
        stream << "(" << location.line << ", " << location.column << ")";
        return stream;
    }


    Buffer::Buffer(std::string const& new_text)
    : text(new_text),
        index(0),
        location()
    {
    }

    Buffer::Buffer(std::istream& stream)
    : text(std::istreambuf_iterator<char>(stream),
            std::istreambuf_iterator<char>(stream)),
        index(0),
        location()
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


}
