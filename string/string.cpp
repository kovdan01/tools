#include "string.h" //NOLINT (modernize-deprecated-headers)
#include <ostream>
#include <stdexcept>
#include <string.h> //NOLINT (modernize-deprecated-headers)

namespace my
{
    /* ---------------- *
     * MEMBER FUNCTIONS *
     * ---------------- */

    //CONSTRUCTORS AND ASSIGNMENT OPERATORS

    string::string() : _data(const_cast<char *>(string::empty_str)) {}

    string::string(char sym)
    {
        _data = new char[2];
        _data[0] = sym;
        _data[1] = char{};
        _size = 1;
        _capacity = 1;
    }

    string::string(const char* str)
    {
        auto* cur_pos = const_cast<char*>(str);
        while(*cur_pos != char{})
            ++cur_pos;
        _size = static_cast<std::size_t>(cur_pos - str);
        _data = new char[_size + 1];
        for (std::size_t i = 0; i <= _size; ++i)
            _data[i] = *(str + i);
        _capacity = _size;
    }

    string::string(const string & str)
    {
        _size = str.size();
        _capacity = _size;
        _data = new char[_capacity + 1];
        for (std::size_t i = 0; i < _size; ++i)
            _data[i] = str[i];
        _data[_size] = char{};
    }

    string::string(string && str) noexcept
    {
        _data = nullptr;
        this->swap(str);
        str._size = 0;
        str._capacity = 0;
        str._data = const_cast<char *>(string::empty_str);
    }

    string& string::operator=(const string& other)
    {
        auto n = other.size();
        this->reserve(n);
        this->_size = n;
        for (std::size_t i = 0; i < n; ++i)
            *(_data + i) = other[i];
        return *this;
    }

    string& string::operator=(string && other) noexcept
    {
        this->swap(other);
        other._size = 0;
        other._capacity = 0;
        if (other._data != empty_str)
        {
            delete[]other._data;
            other._data = const_cast<char *>(string::empty_str);
        }
        return *this;
    }

    string::~string()
    {
        if (_data != empty_str)
            delete[]_data;
    }

    //TODO: assign

    //ELEMENT ACCESS

    char& string::at(std::size_t pos)
    {
        if (pos >= _size)
            throw std::out_of_range("String index out of range");
        return _data[pos];
    }
    const char& string::at(std::size_t pos) const
    {
        if (pos >= _size)
            throw std::out_of_range("String index out of range");
        return _data[pos];
    }

    //ITERATORS

    void swap(string::iterator& a, string::iterator& b) { std::swap(a._sym, b._sym); }

    bool operator==(const string::iterator& lhs, const string::iterator& rhs) { return lhs._sym == rhs._sym; }
    bool operator!=(const string::iterator& lhs, const string::iterator& rhs) { return !(lhs == rhs); }
    bool operator<(const string::iterator& lhs, const string::iterator& rhs) { return lhs._sym < rhs._sym; }
    bool operator>(const string::iterator& lhs, const string::iterator& rhs) { return rhs < lhs; }
    bool operator<=(const string::iterator& lhs, const string::iterator& rhs) { return !(rhs > lhs); }
    bool operator>=(const string::iterator& lhs, const string::iterator& rhs) { return !(lhs < rhs); }

    string::iterator operator+(string::iterator it, string::iterator::difference_type n) { it += n; return it; }
    string::iterator operator+(string::iterator::difference_type n, string::iterator it) { return it + n; }
    string::iterator operator-(string::iterator it, string::iterator::difference_type n) { it -= n; return it; }
    string::iterator::difference_type operator-(const string::iterator& lhs, const string::iterator& rhs) { return (lhs._sym - rhs._sym); }

    void swap(string::const_iterator& a, string::const_iterator& b) { std::swap(a._sym, b._sym); }

    bool operator==(const string::const_iterator& lhs, const string::const_iterator& rhs) { return lhs._sym == rhs._sym; }
    bool operator!=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(lhs == rhs); }
    bool operator<(const string::const_iterator& lhs, const string::const_iterator& rhs) { return lhs._sym < rhs._sym; }
    bool operator>(const string::const_iterator& lhs, const string::const_iterator& rhs) { return rhs < lhs; }
    bool operator<=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(rhs > lhs); }
    bool operator>=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(lhs < rhs); }

    string::const_iterator operator+(string::const_iterator it, string::const_iterator::difference_type n) { it += n; return it; }
    string::const_iterator operator+(string::const_iterator::difference_type n, string::const_iterator it) { return it + n; }
    string::const_iterator operator-(string::const_iterator it, string::const_iterator::difference_type n) { it -= n; return it; }
    string::const_iterator::difference_type operator-(const string::const_iterator& lhs, const string::const_iterator& rhs) { return (lhs._sym - rhs._sym); }

    //CAPACITY

    void string::reserve(std::size_t new_cap)
    {
        if (new_cap == _capacity)
            return;
        if (new_cap > _capacity)
        {
            _capacity = new_cap;
            auto * new_data = new char[_capacity + 1];
            for (std::size_t i = 0; i <= _size; ++i)
                new_data[i] = _data[i];
            std::swap(_data, new_data);
            if (new_data != empty_str)
                delete[]new_data;
        }
        else if (new_cap < _size)
            shrink_to_fit();
    }

    void string::shrink_to_fit()
    {
        _capacity = _size;
        auto * new_data = new char[_capacity + 1];
        for (std::size_t i = 0; i <= _size; ++i)
            new_data[i] = _data[i];
        std::swap(_data, new_data);
        if (new_data != empty_str)
            delete[]new_data;
    }

    //OPERATIONS

    void string::clear() noexcept
    {
        erase(this->begin(), this->end());
    }

    //TODO: insert

    string::iterator string::erase(string::iterator pos)
    {
        return erase(pos, pos + 1);
    }
    string::iterator string::erase(string::iterator first, string::iterator last)
    {
        for (auto it = first; it != this->end() - (last - first); ++it)
        {
            (*it) = *(it + (last - first));
        }
        _size -= static_cast<std::size_t>(last - first);
        _data[_size] = char{};
        return first;
    }
    string& string::erase(std::size_t index, std::size_t count)
    {
        if (index + count > _size)
            throw std::out_of_range("String index out of range");
        erase(iterator(_data + index), iterator(_data + index + count));
        _data[_size] = char{};
        return *this;
    }

    void string::push_back(char sym)
    {
        if (_data == empty_str)
        {
            *this = sym;
            return;
        }
        if (_size + 1 >= _capacity)
            reserve(std::max(static_cast<std::size_t>(_capacity * mul_coeff.num / mul_coeff.den), _capacity + 1)); //NOLINT(readability-static-accessed-through-instance)
        _data[_size] = sym;
        _data[++_size] = char{};
    }

    void string::pop_back()
    {
        erase(this->end() - 1);
    }

    //TODO: append

    string& string::operator+=(const string& str)
    {
        this->reserve(this->_size + str._size);
        for (char c : str)
            this->push_back(c);
        return *this;
    }

    string& string::operator+=(char sym) { return ((*this) += string(sym)); }
    string& string::operator+=(const char* syms) { return ((*this) += string(syms)); }

    //TODO: compare
    //TODO: replace
    //TODO: substr
    //TODO: copy

    void string::resize(std::size_t count) { resize(count, char{}); }

    void string::resize(std::size_t count, char sym)
    {
        if (count <= _size)
        {
            _size = count;
            _data[_size] = char{};
            return;
        }
        for (std::size_t i = 0; i < count - _size; ++i)
            this->push_back(sym);
    }

    void string::swap(string & other) noexcept
    {
        std::swap(this->_data, other._data);
        std::swap(this->_size, other._size);
        std::swap(this->_capacity, other._capacity);
    }

    /* -------------------- *
     * NON-MEMBER FUNCTIONS *
     * -------------------- */

    void swap(string & lhs, string & rhs) noexcept { lhs.swap(rhs); }

    string operator+(const string& lhs, const string& rhs)
    {
        string ans;
        ans.reserve(lhs.size() + rhs.size());
        for (char lh: lhs)
            ans.push_back(lh);
        for (char rh : rhs)
            ans.push_back(rh);
        return ans;
    }

    string operator+(const string& str, const char* syms) { return (str + string(syms)); }
    string operator+(const char* syms, const string& str) { return (string(syms) + str); }
    string operator+(const string& str, char sym) { return (str + string(sym)); }
    string operator+(char sym, const string& str) { return (string(sym) + str); }

    //BLOCK 1: STRING AND STRING

    bool operator==(const string& lhs, const string& rhs)
    {
        if(lhs.size() != rhs.size())
            return false;
        auto n = lhs.size();
        for (std::size_t i = 0; i < n; ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }

    bool operator!=(const string& lhs, const string& rhs) { return !(lhs == rhs); }

    bool operator<(const string& lhs, const string& rhs)
    {
        if (lhs.size() > rhs.size())
            return false;
        if (lhs.size() < rhs.size())
            return true;
        auto n = lhs.size();
        for (std::size_t i = 0; i < n; ++i)
        {
            if (lhs[i] < rhs[i])
                return true;
            if (lhs[i] > rhs[i])
                return false;
        }
        return false;
    }

    bool operator>(const string& lhs, const string& rhs) { return (rhs < lhs); }
    bool operator<=(const string& lhs, const string& rhs) { return !(lhs > rhs); }
    bool operator>=(const string& lhs, const string& rhs) { return !(lhs < rhs); }

    //BLOCK 2: STRING AND CHAR *

    bool operator==(const string& lhs, const char* rhs)
    {
        auto n = strlen(rhs);
        if (n != lhs.size())
            return false;
        for (std::size_t i = 0; i < n; ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }

    bool operator!=(const string& lhs, const char* rhs) { return !(lhs == rhs); }

    bool operator<(const string& lhs, const char* rhs)
    {
        auto n = strlen(rhs);
        if (n > lhs.size())
            return true;
        if (n < lhs.size())
            return false;
        for (std::size_t i = 0; i < n; ++i)
        {
            if (lhs[i] < rhs[i])
                return true;
            if (lhs[i] > rhs[i])
                return false;
        }
        return false;
    }

    bool operator>(const string& lhs, const char* rhs)
    {
        auto n = strlen(rhs);
        if (n < lhs.size())
            return true;
        if (n > lhs.size())
            return false;
        for (std::size_t i = 0; i < n; ++i)
        {
            if (lhs[i] > rhs[i])
                return true;
            if (lhs[i] < rhs[i])
                return false;
        }
        return false;
    }
    bool operator<=(const string& lhs, const char* rhs) { return !(lhs > rhs); }
    bool operator>=(const string& lhs, const char* rhs) { return !(lhs < rhs); }

    //BLOCK 3: CHAR * AND STRING

    bool operator==(const char* lhs, const string& rhs) { return (rhs == lhs); }
    bool operator!=(const char* lhs, const string& rhs) { return (rhs != lhs); }
    bool operator<(const char* lhs, const string& rhs) { return (rhs > lhs); }
    bool operator>(const char* lhs, const string& rhs) { return (rhs < lhs); }
    bool operator<=(const char* lhs, const string& rhs) { return (rhs >= lhs); }
    bool operator>=(const char* lhs, const string& rhs) { return (rhs <= lhs); }

    //OUTPUT

    std::ostream& operator<<(std::ostream& os, string& str)
    {
        for (char c : str)
            os << c;
        return os;
    }
}

