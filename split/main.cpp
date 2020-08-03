#include "split.h"
#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

using namespace std;

template <typename StringType>
void test_algo_string(int max_n_splits, int max_part_len)
{
    using CharType = typename std::remove_reference<decltype (StringType{' '}[0])>::type;
    std::mt19937 prng (std::random_device{}());
    std::uniform_int_distribution<int> int_dist(0, max_part_len);
    std::uniform_int_distribution<CharType> char_dist(std::numeric_limits<CharType>::min(), std::numeric_limits<CharType>::max());
    int n_splits = std::uniform_int_distribution<int>(1, max_n_splits)(prng);
    CharType sep = char_dist(prng), cur_char;
    int cur_len;
    StringType cur, whole;
    std::vector<StringType> v_true;
    v_true.reserve(static_cast<std::size_t>(n_splits));
    for (int i = 0; i < n_splits; ++i)
    {
        cur_len = int_dist(prng);
        cur = StringType{};
        for (int j = 0; j < cur_len; ++j)
        {
            do
            {
                cur_char = char_dist(prng);
            }
            while(cur_char == sep);
            cur += cur_char;
        }
        whole += cur;
        if (i != n_splits - 1)
            whole += sep;
        v_true.emplace_back(cur);
    }
    {
        std::vector<std::basic_string_view<CharType> > v_algo = split::split(whole, sep);
        assert(v_true.size() == v_algo.size());
        for (std::size_t i = 0; i < v_true.size(); ++i)
            assert(v_algo[i] == std::basic_string_view<CharType>(v_true[i].data(), v_true[i].size()));
    }
    {
        std::size_t cur_i = 0;
        split::splitter<CharType> algo(whole, sep);
        for (auto current : algo)
        {
            assert(current == std::basic_string_view<CharType>(v_true[cur_i].data(), v_true[cur_i].size()));
            ++cur_i;
        }
        assert(cur_i == v_true.size());
        cur_i = 0;
        for (auto it = algo.begin(); it != algo.end(); ++it, ++it)
        {
            assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].data(), v_true[cur_i].size()));
            assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].data(), v_true[cur_i].size()));
            cur_i += 2;
        }
    }
}

template <typename CharType>
void test_algo_char(int max_n_splits, int max_part_len, bool with_n)
{
    //Generating data
    std::mt19937 prng (std::random_device{}());
    std::uniform_int_distribution<int> int_dist(0, max_part_len);
    std::uniform_int_distribution<CharType> char_dist(std::numeric_limits<CharType>::min(), std::numeric_limits<CharType>::max());
    std::size_t n_splits = static_cast<std::size_t>(std::uniform_int_distribution<int>(1, max_n_splits)(prng));
    CharType sep, cur_char;
    if (!with_n)
    {
        do sep = char_dist(prng);
        while(sep == CharType{0});
    }
    else
        sep = char_dist(prng);
    std::size_t cur_len, whole_len = 0;
    std::unique_ptr<CharType[]> whole, cur;
    std::vector<std::unique_ptr<CharType[]> > v_true(n_splits);
    std::vector<std::size_t> v_lens(n_splits);
    for (std::size_t i = 0; i < n_splits; ++i)
    {
        cur_len = static_cast<std::size_t>(int_dist(prng));
        whole_len += cur_len;
        cur = std::make_unique<CharType[]>(!with_n ? (cur_len + 1) : cur_len);
        for (std::size_t j = 0; j < cur_len; ++j)
        {
            do cur_char = char_dist(prng);
            while(cur_char == sep || (with_n ? false : (cur_char == CharType{0})));
            cur[j] = cur_char;
        }
        if (!with_n)
            cur[cur_len] = CharType{0};
        if (i != n_splits - 1)
            ++whole_len;
        v_true[i] = std::move(cur);
        v_lens[i] = cur_len;
    }
    whole = std::make_unique<CharType[]>(!with_n ? (whole_len + 1) : whole_len);
    std::size_t cur_pos = 0;
    for (std::size_t i = 0; i < v_true.size(); ++i)
    {
        for (std::size_t j = 0; j < v_lens[i]; ++j)
        {
            assert(cur_pos < whole_len);
            whole[cur_pos] = v_true[i][j];
            ++cur_pos;
        }
        if (i != v_true.size() - 1)
        {
            assert(cur_pos < whole_len);
            whole[cur_pos] = sep;
            ++cur_pos;
        }
    }
    if (!with_n)
        whole[whole_len] = CharType{0};
    //Asserting
    {
        std::vector<std::basic_string_view<CharType> > v_algo = (with_n ? split::split<CharType, std::size_t>(whole.get(), whole_len, sep) : split::split<CharType>(whole.get(), sep));
        assert(v_true.size() == v_algo.size());
        for (std::size_t i = 0; i < v_true.size(); ++i)
            assert(v_algo[i] == (with_n ? std::basic_string_view<CharType>(v_true[i].get(), v_lens[i]) : std::basic_string_view<CharType>(v_true[i].get())));
    }
    {
        std::size_t cur_i = 0;
        if (!with_n)
        {
            split::splitter<CharType> algo(whole.get(), sep);
            for (auto current : algo)
            {
                assert(current == std::basic_string_view<CharType>(v_true[cur_i].get()));
                ++cur_i;
            }
            assert(cur_i == v_true.size());
            cur_i = 0;
            for (auto it = algo.begin(); it != algo.end(); ++it, ++it)
            {
                assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].get()));
                assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].get()));
                cur_i += 2;
            }
        }
        else
        {
            split::splitter<CharType> algo(whole.get(), whole_len, sep);
            for (auto current : algo)
            {
                assert(current == std::basic_string_view<CharType>(v_true[cur_i].get(), v_lens[cur_i]));
                ++cur_i;
            }
            assert(cur_i == v_true.size());
            cur_i = 0;
            for (auto it = algo.begin(); it != algo.end(); ++it, ++it)
            {
                assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].get(), v_lens[cur_i]));
                assert((*it) == std::basic_string_view<CharType>(v_true[cur_i].get(), v_lens[cur_i]));
                cur_i += 2;
            }
        }
    }
}

int main()
{
    for (int i = 1; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            test_algo_string<std::string>(i, j);
            test_algo_char<char>(i, j, true);
            test_algo_char<char>(i, j, false);
            test_algo_string<std::wstring>(i, j);
            test_algo_char<wchar_t>(i, j, true);
            test_algo_char<wchar_t>(i, j, false);
            test_algo_string<std::u16string>(i, j);
            test_algo_char<char16_t>(i, j, true);
            test_algo_char<char16_t>(i, j, false);
            test_algo_string<std::u32string>(i, j);
            test_algo_char<char32_t>(i, j, true);
            test_algo_char<char32_t>(i, j, false);
        }
    }
    return 0;
}
