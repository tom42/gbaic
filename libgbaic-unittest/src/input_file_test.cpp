// MIT License
//
// gbaic: Gameboy Advance Intro Cruncher
// Copyright (c) 2020 Thomas Mathys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "libgbaic_unittest_config.hpp"
#include "input_file.hpp"

namespace libgbaic_unittest
{

using std::filesystem::path;
using std::runtime_error;
using std::vector;

static libgbaic::input_file load_elf_file(const path& filename)
{
    path full_path = LIBGBAIC_UNITTEST_TESTDATA_DIRECTORY / filename;
    return libgbaic::input_file(full_path);
}

static vector<unsigned char> load_binary_file(const path& filename)
{
    path full_path = LIBGBAIC_UNITTEST_TESTDATA_DIRECTORY / filename;

    // Open file, stop it from eating whitespace.
    std::ifstream file(full_path, std::ios::binary);
    BOOST_REQUIRE_MESSAGE(file, "Could not open " + full_path.string());
    file.unsetf(std::ios::skipws);

    // Create vector with sufficient capacity to hold entire file.
    auto filesize = std::filesystem::file_size(full_path);
    vector<unsigned char> data;
    data.reserve(filesize);

    // Read entire file
    data.insert(
        data.begin(),
        std::istream_iterator<unsigned char>(file),
        std::istream_iterator<unsigned char>());
    BOOST_REQUIRE_MESSAGE(file || file.eof(), "Error reading " + full_path.string());
    file.close();

    return data;
}

BOOST_AUTO_TEST_SUITE(input_file_test)

BOOST_AUTO_TEST_CASE(load_elf_file_does_not_exist)
{
    BOOST_CHECK_EXCEPTION(
        libgbaic::input_file f("non-existing-file.elf"),
        runtime_error,
        [](const auto& e) { return boost::iequals("non-existing-file.elf: no such file or directory", e.what()); });
}

BOOST_AUTO_TEST_CASE(load_elf_file_is_invalid)
{
    const char c = 0x42;
    std::stringstream s;
    s.write(&c, 1);

    BOOST_CHECK_EXCEPTION(
        libgbaic::input_file f(s),
        runtime_error,
        [](const auto& e) { return boost::iequals("file is not a valid ELF file", e.what()); });
}

BOOST_AUTO_TEST_CASE(load_elf_lostmarbles)
{
    auto input_file = load_elf_file("lostmarbles.elf");
    auto expected_data = load_binary_file("lostmarbles.bin");

    BOOST_REQUIRE_EQUAL(0x03000000, input_file.entry());
    BOOST_REQUIRE_EQUAL(0x03000000, input_file.load_address());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected_data.begin(), expected_data.end(), input_file.data().begin(), input_file.data().end());
}

BOOST_AUTO_TEST_SUITE_END()

}
