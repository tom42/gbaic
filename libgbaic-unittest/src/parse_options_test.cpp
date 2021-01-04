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
#include <boost/numeric/conversion/cast.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include "options.hpp"

namespace libgbaic_unittest
{

using libgbaic::options;
using std::runtime_error;
using std::string;
using std::vector;

static vector<char> to_vector(const string& s)
{
    return vector<char>(s.c_str(), s.c_str() + s.size() + 1);
}

static options parse_options(const char* command_line)
{
    // Split string into individual arguments and convert them to vector<char>
    vector<vector<char>> vectors;
    vectors.push_back(to_vector("program_name"));
    if (strlen(command_line))
    {
        vector<string> strings;
        boost::split(strings, command_line, boost::is_any_of(" "));
        for (const auto& s : strings)
        {
            vectors.push_back(to_vector(s));
        }
    }

    // Put together an argv array.
    vector<char*> argv;
    for (auto& v : vectors)
    {
        argv.push_back(v.data());
    }

    return libgbaic::parse_options(boost::numeric_cast<int>(argv.size()), argv.data());
}

BOOST_AUTO_TEST_SUITE(parse_options_test)

BOOST_AUTO_TEST_CASE(empty_command_line)
{
    BOOST_CHECK_EXCEPTION(
        parse_options(""),
        runtime_error,
        [](const auto& e) { BOOST_CHECK_EQUAL("No input file given", e.what()); return true; });
}

BOOST_AUTO_TEST_CASE(more_than_one_input_file)
{
    BOOST_CHECK_EXCEPTION(
        parse_options("file1 file2"),
        runtime_error,
        [](const auto& e) { BOOST_CHECK_EQUAL("More than one input file given", e.what()); return true; });
}

BOOST_AUTO_TEST_CASE(one_input_file)
{
    auto options = parse_options("file1");
    BOOST_CHECK_EQUAL("file1", options.input_file());
}

BOOST_AUTO_TEST_SUITE_END()

}