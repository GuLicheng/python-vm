#include "BufferedInputStream.hpp"
#include <iterator>
#include "../Python.hpp"
#include <bit>

static_assert(std::endian::native == std::endian::little);

namespace python
{

    template <typename T>
    T read_basic_type(const char* buffer, int& index)
    {
        constexpr auto offset = sizeof(T);
        T i = *reinterpret_cast<const T*>(buffer + index);
        index += offset;
        return i;
    }

    const char* BufferedInputStream::data() const
    {
        return this->sz_buffer.data() + this->index;
    }

    void BufferedInputStream::skip(int n)
    {
        this->index += n;
        PYTHON_ASSERT(this->index <= this->sz_buffer.size());
    }

    BufferedInputStream::BufferedInputStream(const char* filename)
    {
        this->ifs.open(filename, std::ios::in | std::ios::binary);
        PYTHON_ASSERT(this->ifs.is_open());
        this->sz_buffer = std::string(
            std::istreambuf_iterator<char>(this->ifs),
            std::istreambuf_iterator<char>()
        );
        this->index = 0;
    }

    BufferedInputStream::~BufferedInputStream()
    {
    }

    char BufferedInputStream::read()
    {
        return this->sz_buffer[this->index++];
    }

    int BufferedInputStream::read_int()
    {
        return read_basic_type<int>(this->sz_buffer.data(), this->index);
    }

    double BufferedInputStream::read_double()
    {
        return read_basic_type<double>(this->sz_buffer.data(), this->index);
    }

    void BufferedInputStream::unread()
    {
        this->index--;
    }

}