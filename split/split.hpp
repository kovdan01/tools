#ifndef MY_SPLIT_HPP_
#define MY_SPLIT_HPP_

#include <iterator>
#include <memory>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace my
{

namespace detail
{

template <typename CharType>
std::vector<std::basic_string_view<CharType>> helper(std::basic_string_view<CharType> str, CharType sep)
{
    std::ptrdiff_t cur_sep_ind = -1;
    std::vector<std::basic_string_view<CharType>> ans;
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == sep)
        {
            ans.emplace_back(str.begin() + cur_sep_ind + 1, i - cur_sep_ind - 1);
            cur_sep_ind = static_cast<std::ptrdiff_t>(i);
        }
    }
    ans.emplace_back(str.begin() + cur_sep_ind + 1, str.size() - cur_sep_ind - 1);
    return ans;
}

template <typename CharType>
std::pair<std::basic_string_view<CharType>, std::size_t> helper_one_by_one(std::basic_string_view<CharType> str, CharType sep, std::size_t begin = 0)
{
    for (std::size_t i = begin; i < str.size(); ++i)
        if (str[i] == sep)
            return { std::basic_string_view<CharType>(str.begin() + begin, i - begin), i };
    return { std::basic_string_view<CharType>(str.begin() + begin, str.size() - begin), str.size() };
}

}  // namespace detail

template <typename CharType>
std::vector<std::basic_string_view<CharType>> split(const CharType* str, CharType sep)
{
    return detail::helper({ str }, sep);
}

template <typename CharType, typename SizeType>
std::vector<std::basic_string_view<CharType>> split(const CharType* str, SizeType size, CharType sep)
{
    return detail::helper({ str, size }, sep);
}

template <typename StringType, typename CharType>
std::vector<std::basic_string_view<CharType>> split(const StringType& str, CharType sep)
{
    return detail::helper({ str.data(), str.size() }, sep);
}

template <typename CharType, typename StringType = std::basic_string<CharType> >
class Splitter
{
private:
    std::basic_string_view<CharType> m_str;
    CharType m_sep;

public:
    Splitter() = delete;

    Splitter(const CharType* str, CharType sep)
        : m_str({ str })
        , m_sep(sep)
    {
    }

    Splitter(const CharType* str, std::size_t size, CharType sep)
        : m_str({ str, size })
        , m_sep(sep)
    {
    }

    Splitter(const StringType& str, CharType sep)
        : m_str({ str.data(), str.size() })
        , m_sep(sep)
    {
    }

    class Iterator : std::iterator<std::forward_iterator_tag, std::basic_string_view<CharType>, ptrdiff_t>
    {
    private:
        std::size_t m_begin = 0, m_end = -1;
        Splitter<CharType, StringType>& m_obj;
        bool m_is_calculated = false;

        Iterator(std::size_t begin, std::size_t end, Splitter<CharType, StringType>& obj) noexcept
            : m_begin(begin)
            , m_end(end)
            , m_obj(obj)
        {
        }

        std::basic_string_view<CharType> get_next()
        {
            for (std::size_t i = m_begin; i < m_obj.m_str.size(); ++i)
            {
                if (m_obj.m_str[i] == m_obj.m_sep)
                {
                    m_end = i;
                    return { m_obj.m_str.begin() + m_begin, i - m_begin };
                }
            }
            m_end = m_obj.m_str.size();
            return { m_obj.m_str.begin() + m_begin, m_obj.m_str.size() - m_begin };
        }

    public:
        friend class Splitter<CharType, StringType>;

        Iterator() noexcept = delete;
        Iterator(const Iterator&) noexcept = default;
        Iterator& operator=(const Iterator&) noexcept = default;
        Iterator(Iterator&&) noexcept = default;
        Iterator& operator=(Iterator&&) noexcept = default;
        ~Iterator() = default;

        std::basic_string_view<CharType> operator*()
        {
            if (m_is_calculated)
                return { m_obj.m_str.begin() + m_begin, m_end - m_begin };
            m_is_calculated = true;
            return get_next();
        }

        Iterator& operator++()
        {
            if (!m_is_calculated)
                get_next();
            m_begin = (m_end == m_obj.m_str.size() ? m_obj.m_str.size() : m_end + 1);
            m_is_calculated = false;
            return *this;
        }

        Iterator operator++(int) { auto old = *this; ++(*this); return old; }  // NOLINT (cert-dcl21-cpp)

        friend void swap(Iterator& lhs, Iterator& rhs)
        {
            std::swap(lhs.m_begin, rhs.m_begin);
            std::swap(lhs.m_end, rhs.m_end);
            std::swap(lhs.m_obj, rhs.m_obj);
            std::swap(lhs.m_is_calculated, rhs.m_is_calculated);
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
            return std::tie(lhs.m_begin, lhs.m_end) == std::tie(rhs.m_begin, rhs.m_end);
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }
    };

    Iterator begin()
    {
        return Iterator(0, -1, *this);
    }

    Iterator end()
    {
        return Iterator(m_str.size(), m_str.size(), *this);
    }
};

}  // namespace my

#endif  // MY_SPLIT_HPP_
