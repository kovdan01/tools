#include "split.hpp"

#include <cassert>
#include <limits>
#include <random>
#include <string>
#include <type_traits>
#include <utility>

static std::mt19937 g_prng(std::random_device{}());  // NOLINT (cert-err58-cpp)

template <typename StringType>
void test_algo_string(int max_n_splits, int max_part_len)
{
    using CharType = typename std::remove_reference<decltype (StringType{' '}[0])>::type;

    // Generating data
    std::uniform_int_distribution<int> int_dist(0, max_part_len);
    std::uniform_int_distribution<CharType> char_dist(std::numeric_limits<CharType>::min(), std::numeric_limits<CharType>::max());
    int n_splits = std::uniform_int_distribution<int>(1, max_n_splits)(g_prng);
    CharType sep = char_dist(g_prng), cur_char;
    int cur_len;
    StringType cur, whole;
    std::vector<StringType> v_true;
    v_true.reserve(static_cast<std::size_t>(n_splits));
    for (int i = 0; i < n_splits; ++i)
    {
        cur_len = int_dist(g_prng);
        cur = StringType{};
        for (int j = 0; j < cur_len; ++j)
        {
            do
            {
                cur_char = char_dist(g_prng);
            }
            while (cur_char == sep);
            cur += cur_char;
        }
        whole += cur;
        if (i != n_splits - 1)
            whole += sep;
        v_true.emplace_back(cur);
    }

    // Asserting
    {
        std::vector<std::basic_string_view<CharType>> v_algo = my::split(whole, sep);
        assert(v_true.size() == v_algo.size());
        for (std::size_t i = 0; i < v_true.size(); ++i)
            assert(v_algo[i] == std::basic_string_view<CharType>(v_true[i].data(), v_true[i].size()));
    }
    {
        std::size_t cur_i = 0;
        my::Splitter<CharType> algo(whole, sep);
        for (auto current : algo)
        {
            if (cur_i >= v_true.size())
                assert(false);
            assert(current == std::basic_string_view<CharType>(v_true[cur_i].data(), v_true[cur_i].size()));
            ++cur_i;
        }
        assert(cur_i == v_true.size());
    }
}

template <typename CharType>
void test_algo_char(int max_n_splits, int max_part_len, bool with_n)
{
    // Generating data
    std::uniform_int_distribution<int> int_dist(0, max_part_len);
    std::uniform_int_distribution<CharType> char_dist(std::numeric_limits<CharType>::min(), std::numeric_limits<CharType>::max());
    std::size_t n_splits = static_cast<std::size_t>(std::uniform_int_distribution<int>(1, max_n_splits)(g_prng));
    CharType sep, cur_char;
    if (!with_n)
    {
        do
        {
            sep = char_dist(g_prng);
        }
        while (sep == CharType{0});
    }
    else
    {
        sep = char_dist(g_prng);
    }
    std::size_t cur_len, whole_len = 0;
    std::basic_string<CharType> whole;
    std::vector<std::basic_string<CharType>> v_true(n_splits);
    std::vector<std::size_t> v_lens(n_splits);
    for (std::size_t i = 0; i < n_splits; ++i)
    {
        cur_len = static_cast<std::size_t>(int_dist(g_prng));
        whole_len += cur_len;
        std::basic_string<CharType> cur;
        cur.resize(!with_n ? (cur_len + 1) : cur_len);
        for (std::size_t j = 0; j < cur_len; ++j)
        {
            do
            {
                cur_char = char_dist(g_prng);
            }
            while (cur_char == sep || (with_n ? false : (cur_char == CharType{0})));
            cur[j] = cur_char;
        }
        if (!with_n)
            cur[cur_len] = CharType{0};
        if (i != n_splits - 1)
            ++whole_len;
        v_true[i] = std::move(cur);
        v_lens[i] = cur_len;
    }
    whole.resize(!with_n ? (whole_len + 1) : whole_len);
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

    // Asserting
    {
        std::vector<std::basic_string_view<CharType>> v_algo = (with_n ? my::split<CharType, std::size_t>(whole.data(), whole_len, sep) : my::split<CharType>(whole.data(), sep));
        assert(v_true.size() == v_algo.size());
        for (std::size_t i = 0; i < v_true.size(); ++i)
            assert(v_algo[i] == (with_n ? std::basic_string_view<CharType>(v_true[i].data(), v_lens[i]) : std::basic_string_view<CharType>(v_true[i].data())));
    }

    {
        std::size_t cur_i = 0;
        if (!with_n)
        {
            my::Splitter<CharType> algo(whole.data(), sep);
            for (auto current : algo)
            {
                if (cur_i >= v_true.size())
                    assert(false);
                assert(current == std::basic_string_view<CharType>(v_true[cur_i].data()));
                ++cur_i;
            }
            assert(cur_i == v_true.size());
        }
        else
        {
            my::Splitter<CharType> algo(whole.data(), whole_len, sep);
            for (auto current : algo)
            {
                if (cur_i >= v_true.size())
                    assert(false);
                assert(current == std::basic_string_view<CharType>(v_true[cur_i].data(), v_lens[cur_i]));
                ++cur_i;
            }
            assert(cur_i == v_true.size());
        }
    }
}

int main()
{
    const int max_n_splits_limit = 100;
    const int max_part_len_limit = 100;
    for (int i = 1; i < max_n_splits_limit; ++i)
    {
        for (int j = 0; j < max_part_len_limit; ++j)
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
