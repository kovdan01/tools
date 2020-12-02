#include "string.hpp"

#include <algorithm>
#include <cstring>
#include <ostream>
#include <stdexcept>

namespace my
{

/* ---------------- *
 * MEMBER FUNCTIONS *
 * ---------------- */

// CONSTRUCTORS AND ASSIGNMENT OPERATORS

string::string()
    : m_data(const_cast<char*>(string::m_empty_str))
{
}

string::string(char sym)
    : m_size(1)
    , m_capacity(1)
    , m_data(new char[2])
{
    m_data[0] = sym;
    m_data[1] = char{};
}

string::string(const char* str)
    : m_size(std::strlen(str))
    , m_capacity(m_size)
    , m_data(new char[m_size + 1])
{
    std::copy(str, str + m_size + 1, m_data);
}

string::string(const string& str)
    : m_size(str.size())
    , m_capacity(m_size)
    , m_data(new char[m_capacity + 1])
{
    std::copy(str.m_data, str.m_data + m_size, m_data);
    m_data[m_size] = char{};
}

string::string(string&& str) noexcept
{
    m_data = nullptr;
    this->swap(str);
    str.m_size = 0;
    str.m_capacity = 0;
    str.m_data = const_cast<char*>(string::m_empty_str);
}

string& string::operator=(const string& other)
{
    this->reserve_impl(other.size(), false);
    this->m_size = other.size();
    std::copy(other.m_data, other.m_data + other.size() + 1, m_data);
    return *this;
}

string& string::operator=(string&& other) noexcept
{
    this->swap(other);
    other.m_size = 0;
    other.m_capacity = 0;
    if (other.m_data != m_empty_str)
    {
        delete[] other.m_data;
        other.m_data = const_cast<char*>(string::m_empty_str);
    }
    return *this;
}

string::~string()
{
    if (m_data != m_empty_str)
        delete[] m_data;
}

// TODO: assign

// ELEMENT ACCESS

char& string::at(std::size_t pos)
{
    if (pos >= m_size)
        throw std::out_of_range("String index out of range");
    return m_data[pos];
}
const char& string::at(std::size_t pos) const
{
    if (pos >= m_size)
        throw std::out_of_range("String index out of range");
    return m_data[pos];
}

// ITERATORS

void swap(string::iterator& a, string::iterator& b) { std::swap(a.m_sym, b.m_sym); }

bool operator==(const string::iterator& lhs, const string::iterator& rhs) { return lhs.m_sym == rhs.m_sym; }
bool operator!=(const string::iterator& lhs, const string::iterator& rhs) { return !(lhs == rhs); }
bool operator<(const string::iterator& lhs, const string::iterator& rhs) { return lhs.m_sym < rhs.m_sym; }
bool operator>(const string::iterator& lhs, const string::iterator& rhs) { return rhs < lhs; }
bool operator<=(const string::iterator& lhs, const string::iterator& rhs) { return !(rhs > lhs); }
bool operator>=(const string::iterator& lhs, const string::iterator& rhs) { return !(lhs < rhs); }

string::iterator operator+(string::iterator it, string::iterator::difference_type n) { it += n; return it; }
string::iterator operator+(string::iterator::difference_type n, string::iterator it) { return it + n; }
string::iterator operator-(string::iterator it, string::iterator::difference_type n) { it -= n; return it; }
string::iterator::difference_type operator-(const string::iterator& lhs, const string::iterator& rhs) { return (lhs.m_sym - rhs.m_sym); }

void swap(string::const_iterator& a, string::const_iterator& b) { std::swap(a.m_sym, b.m_sym); }

bool operator==(const string::const_iterator& lhs, const string::const_iterator& rhs) { return lhs.m_sym == rhs.m_sym; }
bool operator!=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(lhs == rhs); }
bool operator<(const string::const_iterator& lhs, const string::const_iterator& rhs) { return lhs.m_sym < rhs.m_sym; }
bool operator>(const string::const_iterator& lhs, const string::const_iterator& rhs) { return rhs < lhs; }
bool operator<=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(rhs > lhs); }
bool operator>=(const string::const_iterator& lhs, const string::const_iterator& rhs) { return !(lhs < rhs); }

string::const_iterator operator+(string::const_iterator it, string::const_iterator::difference_type n) { it += n; return it; }
string::const_iterator operator+(string::const_iterator::difference_type n, string::const_iterator it) { return it + n; }
string::const_iterator operator-(string::const_iterator it, string::const_iterator::difference_type n) { it -= n; return it; }
string::const_iterator::difference_type operator-(const string::const_iterator& lhs, const string::const_iterator& rhs) { return (lhs.m_sym - rhs.m_sym); }

// CAPACITY

void string::reserve_impl(std::size_t new_cap, bool need_copy)
{
    if (new_cap == m_capacity)
        return;

    if (new_cap > m_capacity)
    {
        m_capacity = new_cap;
        char* new_data = new char[m_capacity + 1];
        if (need_copy)
            std::copy(m_data, m_data + m_size + 1, new_data);
        std::swap(m_data, new_data);
        if (new_data != m_empty_str)
            delete[] new_data;
    }
    else if (new_cap < m_size)
    {
        shrink_to_fit_impl(need_copy);
    }
}

void string::reserve(std::size_t new_cap)
{
    this->reserve_impl(new_cap, true);
}

void string::shrink_to_fit_impl(bool need_copy)
{
    if (m_capacity == m_size)
        return;

    m_capacity = m_size;
    char* new_data = new char[m_size + 1];
    if (need_copy)
        std::copy(m_data, m_data + m_size + 1, new_data);
    std::swap(m_data, new_data);
    if (new_data != m_empty_str)
        delete[] new_data;
}

void string::shrink_to_fit()
{
    this->shrink_to_fit_impl(true);
}

// OPERATIONS

void string::clear() noexcept
{
    this->erase(this->begin(), this->end());
}

// TODO: insert

string::iterator string::erase(string::iterator pos)
{
    return this->erase(pos, pos + 1);
}
string::iterator string::erase(string::iterator first, string::iterator last)
{
    for (auto it = first; it != this->end() - (last - first); ++it)
    {
        (*it) = *(it + (last - first));
    }
    m_size -= static_cast<std::size_t>(last - first);
    m_data[m_size] = char{};
    return first;
}
string& string::erase(std::size_t index, std::size_t count)
{
    if (index + count > m_size)
        throw std::out_of_range("String index out of range");
    this->erase(iterator(m_data + index), iterator(m_data + index + count));
    m_data[m_size] = char{};
    return *this;
}

void string::push_back(char sym)
{
    if (m_data == m_empty_str)
    {
        *this = sym;
        return;
    }
    if (m_size + 1 >= m_capacity)
        this->reserve(std::max(static_cast<std::size_t>(m_capacity * string::m_mul_coeff.num / string::m_mul_coeff.den), m_capacity + 1));
    m_data[m_size] = sym;
    m_data[++m_size] = char{};
}

void string::pop_back()
{
    this->erase(this->end() - 1);
}

// TODO: append

string& string::operator+=(const string& str)
{
    this->reserve(this->m_size + str.m_size);
    for (char c : str)
        this->push_back(c);
    return *this;
}

string& string::operator+=(char sym) { return ((*this) += string(sym)); }
string& string::operator+=(const char* syms) { return ((*this) += string(syms)); }

// TODO: compare
// TODO: replace
// TODO: substr
// TODO: copy

void string::resize(std::size_t count) { this->resize(count, char{}); }

void string::resize(std::size_t count, char sym)
{
    if (count <= m_size)
    {
        m_size = count;
        m_data[m_size] = char{};
        return;
    }
    for (std::size_t i = 0; i < count - m_size; ++i)
        this->push_back(sym);
}

void string::swap(string& other) noexcept
{
    std::swap(this->m_data, other.m_data);
    std::swap(this->m_size, other.m_size);
    std::swap(this->m_capacity, other.m_capacity);
}

/* -------------------- *
 * NON-MEMBER FUNCTIONS *
 * -------------------- */

void swap(string& lhs, string& rhs) noexcept { lhs.swap(rhs); }

string operator+(const string& lhs, const string& rhs)
{
    string ans;
    ans.reserve(lhs.size() + rhs.size());
    for (char lh : lhs)
        ans.push_back(lh);
    for (char rh : rhs)
        ans.push_back(rh);
    return ans;
}

string operator+(const string& str, const char* syms) { return (str + string(syms)); }
string operator+(const char* syms, const string& str) { return (string(syms) + str); }
string operator+(const string& str, char sym) { return (str + string(sym)); }
string operator+(char sym, const string& str) { return (string(sym) + str); }

// BLOCK 1: STRING AND STRING

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

// BLOCK 2: STRING AND CHAR *

bool operator==(const string& lhs, const char* rhs)
{
    auto n = std::strlen(rhs);
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
    auto n = std::strlen(rhs);
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
    auto n = std::strlen(rhs);
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

// BLOCK 3: CHAR * AND STRING

bool operator==(const char* lhs, const string& rhs) { return (rhs == lhs); }
bool operator!=(const char* lhs, const string& rhs) { return (rhs != lhs); }
bool operator<(const char* lhs, const string& rhs) { return (rhs > lhs); }
bool operator>(const char* lhs, const string& rhs) { return (rhs < lhs); }
bool operator<=(const char* lhs, const string& rhs) { return (rhs >= lhs); }
bool operator>=(const char* lhs, const string& rhs) { return (rhs <= lhs); }

// OUTPUT

std::ostream& operator<<(std::ostream& os, string& str)
{
    for (char c : str)
        os << c;
    return os;
}

}  // namespace my
