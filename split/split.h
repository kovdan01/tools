#ifndef SPLIT_H
#define SPLIT_H

#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace split
{
    namespace detail
    {
        template <typename CharType>
        std::vector<std::basic_string_view<CharType> > helper(std::basic_string_view<CharType> str, CharType sep)
        {
            std::ptrdiff_t cur_sep_ind = -1;
            std::vector<std::basic_string_view<CharType> > ans;
            for (std::size_t i = 0; i < str.size(); ++i)
            {
                if (str[i] == sep)
                {
                    ans.emplace_back(std::basic_string_view<CharType>{str.begin() + cur_sep_ind + 1, i - cur_sep_ind - 1});
                    cur_sep_ind = static_cast<std::ptrdiff_t>(i);
                }
            }
            ans.emplace_back(std::basic_string_view<CharType>{str.begin() + cur_sep_ind + 1, str.size() - cur_sep_ind - 1});
            return ans;
        }

        template <typename CharType>
        std::pair<std::basic_string_view<CharType>, std::size_t> helper_one_by_one(std::basic_string_view<CharType> str, CharType sep, std::size_t begin = 0)
        {
            for (std::size_t i = begin; i < str.size(); ++i)
                if (str[i] == sep)
                    return std::pair{std::basic_string_view<CharType>(str.begin() + begin, i - begin), i};
            return std::pair{std::basic_string_view<CharType>(str.begin() + begin, str.size() - begin), str.size()};
        }
    }

    template <typename CharType>
    std::vector<std::basic_string_view<CharType> > split(const CharType str[], CharType sep)
    {
        return detail::helper({str}, sep);
    }

    template <typename CharType, typename SizeType>
    std::vector<std::basic_string_view<CharType> > split(const CharType str[], SizeType size, CharType sep)
    {
        return detail::helper({str, size}, sep);
    }

    template <typename StringType, typename CharType>
    std::vector<std::basic_string_view<CharType> > split(const StringType & str, CharType sep)
    {
        return detail::helper({str.data(), str.size()}, sep);
    }

    template <typename CharType, typename StringType = std::basic_string<CharType> >
    class splitter
    {
        std::basic_string_view<CharType> _str;
        CharType _sep;

    public:

        splitter() = delete;
        splitter(const CharType str[], CharType sep) : _str({str}), _sep(sep) {}
        splitter(const CharType str[], std::size_t size, CharType sep) : _str({str, size}), _sep(sep) {}
        splitter(const StringType & str, CharType sep) : _str({str.data(), str.size()}), _sep(sep) {}

        struct iterator : std::iterator<std::forward_iterator_tag, std::basic_string_view<CharType>, ptrdiff_t>
        {
        private:
            std::ptrdiff_t _begin = 0, _end = -1;
            splitter<CharType, StringType> * _obj;
            bool is_calculated = false;

            std::basic_string_view<CharType> get_next()
            {
                for (std::size_t i = _begin; i < _obj->_str.size(); ++i)
                    if (_obj->_str[i] == _obj->_sep)
                    {
                        _end = i;
                        return std::basic_string_view<CharType>(_obj->_str.begin() + _begin, i - _begin);
                    }
                _end = _obj->_str.size();
                return std::basic_string_view<CharType>(_obj->_str.begin() + _begin, _obj->_str.size() - _begin);
            }

        public:
            iterator() noexcept = default;
            iterator(std::ptrdiff_t b, std::ptrdiff_t e, splitter<CharType, StringType> * o) noexcept : _begin(b), _end(e), _obj(o) {}
            iterator(const iterator&) noexcept = default;
            iterator& operator=(const iterator&) noexcept = default;
            ~iterator() = default;
            std::basic_string_view<CharType> operator*()
            {
                if (is_calculated)
                    return std::basic_string_view<CharType>(_obj->_str.begin() + _begin, _end - _begin);
                is_calculated = true;
                return get_next();
            }
            iterator& operator++()
            {
                if (!is_calculated)
                    get_next();
                _begin = (_end == _obj->_str.size() ? _obj->_str.size() : _end + 1);
                is_calculated = false;
                return *this;
            }
            iterator operator++(int) { auto old = *this; ++(*this); return old; } //NOLINT (cert-dcl21-cpp)

            friend void swap(iterator& a, iterator& b)
            {
                std::swap(a._begin, b._begin);
                std::swap(a._end, b._end);
                std::swap(a._obj, b._obj);
                std::swap(a.is_calculated, b.is_calculated);
            }
            friend bool operator==(const iterator& lhs, const iterator& rhs)
            {
                return ((lhs._begin == rhs._begin) && (lhs._end == rhs._end));
            }
            friend bool operator!=(const iterator& lhs, const iterator& rhs)
            {
                return !(lhs == rhs);
            }
        };

        iterator begin()
        {
            return iterator(0, -1, this);
        }

        iterator end()
        {
            return iterator(_str.size(), _str.size(), this);
        }
    };
}

#endif // SPLIT_H
