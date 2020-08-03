#include "string.h" //NOLINT (modernize-deprecated-headers)
#include <string.h> //NOLINT (modernize-deprecated-headers)
#include <iostream>
#include <utility>
#include <string>
#include <cassert>

using namespace std;

void check_at(const std::string & std_s, const my::string & my_s, std::size_t pos)
{
    assert(my_s.at(pos) == std_s.at(pos));
}

void check_at(std::string & std_s, my::string & my_s, std::size_t pos, char c)
{
    my_s.at(pos) = c;
    std_s.at(pos) = c;
    assert(std_s.at(pos) == my_s.at(pos));
}

void check_brackets(const std::string & std_s, const my::string & my_s, std::size_t pos)
{
    assert(std_s[pos] == my_s[pos]);
}

void check_front(const std::string & std_s, const my::string & my_s)
{
    assert(std_s.front() == my_s.front());
}

void check_front(std::string & std_s, my::string & my_s, char c)
{
    std_s.front() = c;
    my_s.front() = c;
    assert(std_s.front() == my_s.front());
}

void check_back(const std::string & std_s, const my::string & my_s)
{
    assert(std_s.back() == my_s.back());
}

void check_back(std::string & std_s, my::string & my_s, char c)
{
    std_s.back() = c;
    my_s.back() = c;
    assert(std_s.back() == my_s.back());
}

void check_data(const std::string & std_s, const my::string & my_s)
{
    auto std_data = std_s.data();
    auto my_data = my_s.data();
    auto n = strlen(std_data), m = strlen(my_data);
    assert(n == m);
    for(std::size_t i = 0; i < n; ++i)
        assert(std_data[i] == my_data[i]);
}

void check_c_str(const std::string & std_s, const my::string & my_s)
{
    auto std_c_str = std_s.c_str();
    auto my_c_str = my_s.c_str();
    auto n = strlen(std_c_str), m = strlen(my_c_str);
    assert(n == m);
    for(std::size_t i = 0; i < n; ++i)
        assert(std_c_str[i] == my_c_str[i]);
}

void check_begin(const my::string & my_s)
{
    if (!my_s.empty())
        assert(*my_s.begin() == my_s[0]);
}

void check_rbegin(const my::string & my_s)
{
    if (!my_s.empty())
        assert(*my_s.rbegin() == my_s[my_s.size() - 1]);
}

void check_end(const my::string & my_s)
{
    if (!my_s.empty())
        assert(*(my_s.end() - 1) == my_s[my_s.size() - 1]);
}

void check_rend(const my::string & my_s)
{
    if (!my_s.empty())
        assert(*(my_s.rend() - 1) == my_s[0]);
}

void check_empty(const std::string & std_s, const my::string & my_s)
{
    assert(std_s.empty() == my_s.empty());
}

void check_size_length_capacity(const std::string & std_s, const my::string & my_s)
{
    assert(std_s.size() == my_s.size());
    assert(std_s.length() == my_s.length());
    assert(my_s.size() <= my_s.capacity());
}

void check_reserve(my::string & my_s, std::size_t new_cap)
{
    auto old_size = my_s.size();
    my_s.reserve(new_cap);
    assert(my_s.size() == old_size);
    assert(my_s.capacity() >= new_cap);
}

void check_shrink_to_fit(my::string & my_s)
{
    auto old_size = my_s.size();
    my_s.shrink_to_fit();
    assert(my_s.size() == old_size);
    assert(my_s.size() == my_s.capacity());
    assert(my_s.size() == strlen(my_s.data()));
}

void check_clear(my::string & my_s)
{
    my_s.clear();
    assert(my_s.empty());
    assert(strlen(my_s.data()) == 0);
}

void check_erase(std::string & std_s, std::string::iterator std_it, my::string & my_s, my::string::iterator my_it)
{
    my_s.erase(my_it);
    std_s.erase(std_it);
    check_data(std_s, my_s);
}

void check_erase(std::string & std_s, std::string::iterator std_it1, std::string::iterator std_it2, my::string & my_s, my::string::iterator my_it1, my::string::iterator my_it2)
{
    my_s.erase(my_it1, my_it2);
    std_s.erase(std_it1, std_it2);
    check_data(std_s, my_s);
}

void check_erase(std::string & std_s, my::string & my_s, std::size_t ind, std::size_t cnt)
{
    my_s.erase(ind, cnt);
    std_s.erase(ind, cnt);
    check_data(std_s, my_s);
}

void check_push_back(std::string & std_s, my::string & my_s, char c)
{
    my_s.push_back(c);
    std_s.push_back(c);
    check_data(std_s, my_s);
}

void check_pop_back(std::string & std_s, my::string & my_s)
{
    std_s.pop_back();
    my_s.pop_back();
    check_data(std_s, my_s);
}

void check_short_plus(std::string & std_s, my::string & my_s, char c)
{
    std_s += c;
    my_s += c;
    check_data(std_s, my_s);
}

void check_short_plus(std::string & std_s, my::string & my_s, const char* c)
{
    std_s += c;
    my_s += c;
    check_data(std_s, my_s);
}

void check_short_plus(std::string & std_s, const std::string & std_other, my::string & my_s, const my::string & my_other)
{
    std_s += std_other;
    my_s += my_other;
    check_data(std_s, my_s);
}

void check_resize(std::string & std_s, my::string & my_s, std::size_t n, char c)
{
    std_s.resize(n, c);
    my_s.resize(n, c);
    check_data(std_s, my_s);
}

void check_resize(std::string & std_s, my::string & my_s, std::size_t n)
{
    std_s.resize(n);
    my_s.resize(n);
    check_data(std_s, my_s);
}

void check_swap(std::string & std_s, std::string & std_other, my::string & my_s, my::string & my_other)
{
    check_data(std_s, my_s);
    check_data(std_other, my_other);
    swap(std_s, std_other);
    swap(my_s, my_other);
    check_data(std_s, my_s);
    check_data(std_other, my_other);
}

void check_binaries(const std::string & std_s, const std::string & std_other, const my::string & my_s, const my::string & my_other)
{
    check_data(std_s, my_s);
    check_data(std_other, my_other);
    assert((std_s == std_other) == (my_s == my_other));
    assert((std_s != std_other) == (my_s != my_other));
    assert((std_s < std_other) == (my_s < my_other));
    assert((std_s > std_other) == (my_s > my_other));
    assert((std_s <= std_other) == (my_s <= my_other));
    assert((std_s >= std_other) == (my_s >= my_other));
}

void check_binaries(const std::string & std_s, const my::string & my_s, const char * other)
{
    check_data(std_s, my_s);
    auto n = strlen(other), m = strlen(other);
    assert(n == m);
    for(std::size_t i = 0; i < n; ++i)
        assert(other[i] == other[i]);

    assert((std_s == other) == (my_s == other));
    assert((std_s != other) == (my_s != other));
    assert((std_s < other) == (my_s < other));
    assert((std_s > other) == (my_s > other));
    assert((std_s <= other) == (my_s <= other));
    assert((std_s >= other) == (my_s >= other));

    assert((other == std_s) == (other == my_s));
    assert((other != std_s) == (other != my_s));
    assert((other < std_s) == (other < my_s));
    assert((other > std_s) == (other > my_s));
    assert((other <= std_s) == (other <= my_s));
    assert((other >= std_s) == (other >= my_s));
}

int main()
{
    {
        my::string my_s;
        std::string std_s;
        char c = 'a';
        for (int i = 0; i < 26; ++i)
        {
            check_push_back(std_s, my_s, static_cast<char>(c + i));
            check_size_length_capacity(std_s, my_s);
            check_empty(std_s, my_s);
            check_front(std_s, my_s);
            check_back(std_s, my_s);
        }
        check_at(std_s, my_s, 2, 'g');
        for(;;)
        {
            try
            {
                check_at(std_s, my_s, 100, 'b');
            }
            catch(const std::out_of_range&)
            {
                break;
            }
            assert(false);
        }
        for(;;)
        {
            try
            {
                check_at(std_s, my_s, 50);
            }
            catch(const std::out_of_range&)
            {
                break;
            }
            assert(false);
        }
        check_shrink_to_fit(my_s);
        check_data(std_s, my_s);
        check_size_length_capacity(std_s, my_s);
        check_empty(std_s, my_s);
        check_front(std_s, my_s);
        check_front(std_s, my_s, 'c');
        check_back(std_s, my_s);
        check_back(std_s, my_s, 'a');
        {
            auto it1 = my_s.begin(), it2 = my_s.end();
            swap(it1, it2);
            assert(it1 == my_s.end() && it2 == my_s.begin());
        }
        {
            auto it1 = my_s.rbegin(), it2 = my_s.rend();
            swap(it1, it2);
            assert(it1 == my_s.rend() && it2 == my_s.rbegin());
        }
        {
            auto it1 = my_s.cbegin(), it2 = my_s.cend();
            swap(it1, it2);
            assert(it1 == my_s.cend() && it2 == my_s.cbegin());
        }
        {
            auto it1 = my_s.crbegin(), it2 = my_s.crend();
            swap(it1, it2);
            assert(it1 == my_s.crend() && it2 == my_s.crbegin());
        }
        assert(my_s.begin() < my_s.end());
        assert(my_s.rbegin() <= my_s.rend());
        assert(!(my_s.cbegin() >= my_s.cend()));
        assert(static_cast<std::ptrdiff_t>(my_s.size()) + my_s.crbegin() == my_s.crend());
        assert(!(my_s.begin() > my_s.end()));
        assert(++my_s.begin() == my_s.begin() + 1); //NOLINT(bugprone-assert-side-effect)
        assert(my_s.begin()++ == my_s.begin()); //NOLINT(bugprone-assert-side-effect)
        assert(my_s.cbegin() != my_s.cend());
        assert(my_s.begin() == my_s.end() - static_cast<std::ptrdiff_t>(my_s.size()));
        assert(my_s.rbegin() == my_s.rend() - static_cast<std::ptrdiff_t>(my_s.size()));
        assert(my_s.cbegin()-- == my_s.cend() - static_cast<std::ptrdiff_t>(my_s.size())); //NOLINT(bugprone-assert-side-effect)
        assert(my_s.crbegin() == my_s.crend() - static_cast<std::ptrdiff_t>(my_s.size()));
        assert(my_s.begin() + static_cast<std::ptrdiff_t>(my_s.size()) - 1 == --my_s.end()); //NOLINT(bugprone-assert-side-effect)
        assert(static_cast<std::ptrdiff_t>(my_s.size()) + my_s.rbegin() == my_s.rend()--); //NOLINT(bugprone-assert-side-effect)
        assert(my_s.cbegin() >= my_s.cend() - static_cast<std::ptrdiff_t>(my_s.size()));
        assert(my_s.crbegin() <= my_s.crend() - static_cast<std::ptrdiff_t>(my_s.size()));
        while(!std_s.empty())
        {
            check_back(std_s, my_s);
            check_front(std_s, my_s);
            check_pop_back(std_s, my_s);
            check_size_length_capacity(std_s, my_s);
            check_empty(std_s, my_s);
        }
    }
    {
        my::string my_s1("abcdef"), my_s2("abcdeh"), my_s3("abc"), my_s4("abcdefg");
        std::string std_s1("abcdef"), std_s2("abcdeh"), std_s3("abc"), std_s4("abcdefg");
        const char * other_1 = "abcdef", *other_2 = "abcdeh", *other_3 = "abc", *other_4 = "abcdefg", *other_5 = "aacdef";
        check_binaries(std_s1, std_s1, my_s1, my_s1);
        check_binaries(std_s1, std_s2, my_s1, my_s2);
        check_binaries(std_s1, std_s3, my_s1, my_s3);
        check_binaries(std_s1, std_s4, my_s1, my_s4);
        check_binaries(std_s1, my_s1, other_1);
        check_binaries(std_s1, my_s1, other_2);
        check_binaries(std_s1, my_s1, other_3);
        check_binaries(std_s1, my_s1, other_4);
        check_binaries(std_s1, my_s1, other_5);
        check_data(std_s1 + std_s2, my_s1 + my_s2);
        check_data(std_s1 + 'a', my_s1 + 'a');
        check_data('a' + std_s2, 'a' + my_s2);
        check_data("std" + std_s2, "std" + my_s2);
        check_data(std_s1 + "std", my_s1 + "std");
        check_c_str(std_s1, my_s1);
    }
    {
        my::string my_s;
        std::string std_s;
        const char * other = "abcde";
        check_short_plus(std_s, std::string(other), my_s, my::string(other));
        check_short_plus(std_s, my_s, 'a');
        check_erase(std_s, my_s, 0, 3);
        for (;;)
        {
            try
            {
                check_erase(std_s, my_s, 0, 100);
            }
            catch(const std::out_of_range&)
            {
                break;
            }
            assert(false);
        }
        check_short_plus(std_s, my_s, other);
        check_erase(std_s, std_s.begin() + 1, std_s.begin() + 5, my_s, my_s.begin() + 1, my_s.begin() + 5);
        check_short_plus(std_s, my_s, other);
        check_at(std_s, my_s, 3);
        check_reserve(my_s, 2);
        check_begin(my_s);
        check_rbegin(my_s);
        check_end(my_s);
        check_rend(my_s);
        check_brackets(std_s, my_s, 3);
        check_erase(std_s, std_s.begin() + 1, my_s, my_s.begin() + 1);
        check_resize(std_s, my_s, 20);
        check_resize(std_s, my_s, 3, 'B');
        std::string std_other = other;
        my::string my_other = other;
        check_swap(std_s, std_other, my_s, my_other);
        check_clear(my_s);
    }
    {
        my::string my_s1("abc"), my_s2("def");
        check_reserve(my_s1, 3);
        my_s2 = std::move(my_s1);
        assert(my_s2 == "abc");
        assert(my_s1.empty()); //NOLINT(bugprone-use-after-move)
    }
    {
        my::string my_s1("abc"), my_s2(std::move(my_s1));
        assert(my_s2 == "abc");
        assert(my_s1.empty());
    }
    {
        my::string my_s1("abc"), my_s2;
        my_s2 = std::move(my_s1);
        assert(my_s2 == "abc");
        assert(my_s1.empty()); //NOLINT(bugprone-use-after-move)
    }
    {
        my::string my_s1("abc"), my_s2(my_s1);
        assert(my_s2 == my_s1);
        my_s2[0] = 'b';
        assert(my_s2 != my_s1);
        assert(!my_s1.empty());
    }
    {
        my::string my_s1("abc"), my_s2;
        my_s2 = my_s1;
        assert(my_s2 == my_s1);
        my_s2[0] = 'b';
        assert(my_s2 != my_s1);
        assert(!my_s1.empty());
    }
}
