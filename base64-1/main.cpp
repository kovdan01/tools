#include "base64.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

void print_read_error(int i, char type)
{
    std::cerr << "Error while reading from file " << i;
    switch(type)
    {
    case 't':
        std::cerr << ".test";
        break;
    case 'a':
        std::cerr << ".ans";
        break;
    case 'i':
        std::cerr << ".invalid";
        break;
    }
    std::cerr << '\n';
}

void print_open_error(int i, char type)
{
    std::cerr << "Unable to open test number " << i << ". ";
    std::cerr << "Error while reading from file " << i;
    switch(type)
    {
    case 't':
        std::cerr << "Please put " << i << ".test and " << i << ".ans files in ./test/valid/ folder.";
        break;
    case 'i':
        std::cerr << "Please put " << i << ".invalid file in ./test/invalid/ folder.";
        break;
    }
    std::cerr << "\nTesting will continue from the next test.\n";
}

int main()
{
    /* These 50 tests are placed in ./tests/valid/ folder.
     * In *.test files input data is placed.
     * In relevant *.ans files correct base64 is placed. */
    for (int i = 0; i < 50; ++i)
    {
        std::ifstream test("tests/valid/" + std::to_string(i) + ".test", std::ios::binary | std::ios::ate),
                      ans("tests/valid/" + std::to_string(i) + ".ans", std::ios::binary | std::ios::ate);
        if (!test.is_open() || !ans.is_open())
        {
            print_open_error(i, 't');
            continue;
        }
        std::size_t test_size, ans_size;
        std::vector<base64::byte> test_bytes, decoded_bytes;
        std::string ans_str, encoded_str;
        try
        {
            test_size = static_cast<std::size_t>(test.tellg());
            if (!test.seekg (0, std::ifstream::beg))
            {
                print_read_error(i, 't');
                continue;
            }
            test_bytes.resize(test_size);
            if (!test.read(reinterpret_cast<char *>(&test_bytes[0]), static_cast<std::streamsize>(test_size)))
            {
                print_read_error(i, 't');
                continue;
            }

            ans_size = static_cast<std::size_t>(ans.tellg());
            if (!ans.seekg (0, std::ifstream::beg))
            {
                print_read_error(i, 'a');
                continue;
            }
            ans_str.resize(ans_size);
            if (!ans.read(&ans_str[0], static_cast<std::streamsize>(ans_size)))
            {
                print_read_error(i, 'a');
                continue;
            }

            encoded_str = base64::encode(test_bytes);
            assert(ans_size == encoded_str.size());
            for (std::size_t j = 0; j < ans_size; ++j)
                assert(ans_str[j] == encoded_str[j]);

            try
            {
                decoded_bytes = base64::decode(ans_str);
            }
            catch (const std::invalid_argument&) //Exception in decode function
            {
                assert(false);
            }
            catch (const std::exception& e) //Exception in allocating memory for vector or smth else not connected to decode function
            {
                std::cerr << e.what() << '\n';
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        assert(test_size == decoded_bytes.size());
        for (std::size_t j = 0; j < test_size; ++j)
            assert(test_bytes[j] == decoded_bytes[j]);
    }
    /* These 10 tests are placed in ./tests/invalid/ folder.
     * In *.invalid files corrupted base64 data is placed.
     * Decode function must throw an std::invalid_argument exception in these cases */
    for (int i = 0; i < 10; ++i)
    {
        std::ifstream test("tests/invalid/" + std::to_string(i) + ".invalid", std::ios::binary | std::ios::ate);
        if (!test.is_open())
        {
            print_open_error(i, 'i');
            continue;
        }
        try
        {
            auto test_size = test.tellg();
            if (!test.seekg (0, std::ifstream::beg))
            {
                print_read_error(i, 'i');
                continue;
            }
            std::string test_str;
            test_str.resize(static_cast<std::size_t>(test_size));
            if(!test.read(&test_str[0], test_size))
            {
                print_read_error(i, 'i');
                continue;
            }

            try
            {
                base64::decode(test_str);
            }
            catch (const std::invalid_argument&) //Exception in decode function
            {
                continue;
            }
            catch (const std::exception& e) //Exception in allocating memory for vector or smth else not connected to decode function
            {
                std::cerr << e.what() << '\n';
            }
            assert(false);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
