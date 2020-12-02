#ifndef MY_STRING_H
#define MY_STRING_H

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ratio>

namespace my
{

class string
{
private:
    std::size_t m_size = 0, m_capacity = 0;
    char* m_data;
    static constexpr std::ratio<3, 2> m_mul_coeff{};
    static constexpr const char* m_empty_str = "";

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    /* ---------------- *
     * MEMBER FUNCTIONS *
     * ---------------- */

    // CONSTRUCTORS, DESTRUCTOR AND ASSIGNMENT OPERATORS

    string();
    string(char sym);
    string(const char* str);
    string(const string& str);
    string(string&& str) noexcept;
    ~string();
    string& operator=(const string& other);
    string& operator=(string&& other) noexcept;
    // TODO: assign

    // ELEMENT ACCESS

    char& at(std::size_t pos);
    const char& at(std::size_t pos) const;
    char& operator[](std::size_t pos) { return *(m_data + pos); }
    const char& operator[](std::size_t pos) const { return *(m_data + pos); }
    char& front() { return m_data[0]; }
    const char& front() const { return m_data[0]; }
    char& back() { return m_data[m_size - 1]; }
    const char& back() const { return m_data[m_size - 1]; }
    char* data() const noexcept { return m_data; }
    char* c_str() const noexcept { return m_data; }

    // ITERATORS
    struct iterator : std::iterator<std::random_access_iterator_tag, char, ptrdiff_t>
    {
    private:
        char* m_sym;
    public:
        iterator(char* sym) noexcept : m_sym(sym) {}
        iterator() noexcept = default;
        iterator(const iterator&) noexcept = default;
        iterator& operator=(const iterator&) noexcept = default;
        ~iterator() = default;

        reference operator*() const { return (*m_sym); }
        pointer operator->() const;
        reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }

        iterator& operator++() { ++m_sym; return *this; }
        iterator operator++(int) { auto old = *this; ++(*this); return old; }  // NOLINT (cert-dcl21-cpp)
        iterator& operator--() { --m_sym; return *this; }
        iterator operator--(int) { auto old = *this; --(*this); return old; }  // NOLINT (cert-dcl21-cpp)
        iterator& operator+=(difference_type n) { m_sym += n; return *this; }
        iterator& operator-=(difference_type n) { m_sym -= n; return *this; }

        friend void swap(iterator& a, iterator& b);
        friend bool operator==(const iterator& lhs, const iterator& rhs);
        friend bool operator!=(const iterator& lhs, const iterator& rhs);
        friend bool operator<(const iterator& lhs, const iterator& rhs);
        friend bool operator>(const iterator& lhs, const iterator& rhs);
        friend bool operator<=(const iterator& lhs, const iterator& rhs);
        friend bool operator>=(const iterator& lhs, const iterator& rhs);
        friend iterator operator+(iterator it, iterator::difference_type n);
        friend iterator operator+(iterator::difference_type n, iterator it);
        friend iterator operator-(iterator it, iterator::difference_type n);
        friend iterator::difference_type operator-(const iterator& lhs, const iterator& rhs);
    };

    using reverse_iterator = std::reverse_iterator<iterator>;

    iterator begin() const noexcept { return iterator(m_data); }
    iterator end() const noexcept { return iterator(m_data + m_size); }
    reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->end()); }
    reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->begin()); }

    struct const_iterator : std::iterator<std::random_access_iterator_tag, const char, ptrdiff_t>
    {
    private:
        const char* m_sym;
    public:
        const_iterator(const char* sym) noexcept : m_sym(sym) {}
        const_iterator() noexcept = default;
        const_iterator(const const_iterator&) noexcept = default;
        const_iterator& operator=(const const_iterator&) noexcept = default;
        ~const_iterator() = default;

        reference operator*() const { return (*m_sym); }
        pointer operator->() const;
        reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }

        const_iterator& operator++() { ++m_sym; return *this; }
        const_iterator operator++(int) { auto old = *this; ++(*this); return old; }  // NOLINT (cert-dcl21-cpp)
        const_iterator& operator--() { --m_sym; return *this; }
        const_iterator operator--(int) { auto old = *this; --(*this); return old; }  // NOLINT (cert-dcl21-cpp)
        const_iterator& operator+=(difference_type n) { m_sym += n; return *this; }
        const_iterator& operator-=(difference_type n) { m_sym -= n; return *this; }

        friend void swap(const_iterator& a, const_iterator& b);
        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator<(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator>(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator<=(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator>=(const const_iterator& lhs, const const_iterator& rhs);
        friend const_iterator operator+(const_iterator it, const_iterator::difference_type n);
        friend const_iterator operator+(const_iterator::difference_type n, const_iterator it);
        friend const_iterator operator-(const_iterator it, const_iterator::difference_type n);
        friend const_iterator::difference_type operator-(const const_iterator& lhs, const const_iterator& rhs);
    };

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const_iterator cbegin() const noexcept { return const_iterator(m_data); }
    const_iterator cend() const noexcept { return const_iterator(m_data + m_size); }
    const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(this->cend()); }
    const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(this->cbegin()); }

    // CAPACITY

    bool empty() const noexcept { return (m_size == 0); }
    std::size_t size() const noexcept { return m_size; }
    std::size_t length() const noexcept { return m_size; }
    void reserve(std::size_t new_cap = 0);
    std::size_t capacity() const noexcept { return m_capacity; }
    void shrink_to_fit();

    // OPERATIONS

    void clear() noexcept;
    // TODO: insert
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    string& erase(std::size_t index = 0, std::size_t count = npos);
    void push_back(char sym);
    void pop_back();
    // TODO: append
    string& operator+=(const string& str);
    string& operator+=(char sym);
    string& operator+=(const char* syms);
    // TODO: compare
    // TODO: replace
    // TODO: substr
    // TODO: copy
    void resize(std::size_t count);
    void resize(std::size_t count, char sym);
    void swap(string& other) noexcept;

    /* -------------------- *
     * NON-MEMBER FUNCTIONS *
     * -------------------- */

    friend void swap(string& lhs, string& rhs) noexcept;

    friend bool operator==(const string& lhs, const string& rhs);
    friend bool operator!=(const string& lhs, const string& rhs);
    friend bool operator<(const string& lhs, const string& rhs);
    friend bool operator>(const string& lhs, const string& rhs);
    friend bool operator<=(const string& lhs, const string& rhs);
    friend bool operator>=(const string& lhs, const string& rhs);

    friend bool operator==(const string& lhs, const char* rhs);
    friend bool operator!=(const string& lhs, const char* rhs);
    friend bool operator<(const string& lhs, const char* rhs);
    friend bool operator>(const string& lhs, const char* rhs);
    friend bool operator<=(const string& lhs, const char* rhs);
    friend bool operator>=(const string& lhs, const char* rhs);

    friend bool operator==(const char* lhs, const string& rhs);
    friend bool operator!=(const char* lhs, const string& rhs);
    friend bool operator<(const char* lhs, const string& rhs);
    friend bool operator>(const char* lhs, const string& rhs);
    friend bool operator<=(const char* lhs, const string& rhs);
    friend bool operator>=(const char* lhs, const string& rhs);

    friend string operator+(const string& lhs, const string& rhs);
    friend string operator+(const string& str, const char* syms);
    friend string operator+(const char* syms, const string& str);
    friend string operator+(const string& str, char sym);
    friend string operator+(char sym, const string& str);

    // OUTPUT

    friend std::ostream& operator<<(std::ostream& os, string& str);

private:
    void reserve_impl(std::size_t new_cap, bool need_copy);
    void shrink_to_fit_impl(bool need_copy);
};

}  // namespace my

#endif  // MY_STRING_H
