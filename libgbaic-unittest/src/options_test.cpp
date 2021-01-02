// MIT License
//
// gbaic - Gameboy Advance Intro Cruncher
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
#include <string>
#include <vector>
#include "options.hpp"

namespace libgbaic_unittest
{

using libgbaic::options;
using std::vector;
using std::string;

static vector<char> to_vector(const string& s)
{
    return vector<char>(s.c_str(), s.c_str() + s.size() + 1);
}

static options parse_options(const char* command_line)
{
    // Split string at whitespace
    vector<string> strings;
    boost::split(strings, command_line, boost::is_any_of(" "));

    // Convert strings to vector<char>
    vector<vector<char>> vectors;
    for (const auto& s : strings)
    {
        vectors.push_back(to_vector(s));
    }

    // Put together an argv array.
    vector<char*> argv;
    for (auto& v : vectors)
    {
        argv.push_back(v.data());
    }

    return options(boost::numeric_cast<int>(argv.size()), argv.data());
}

BOOST_AUTO_TEST_SUITE(options_test)

BOOST_AUTO_TEST_CASE(empty_command_line)
{
    auto options = parse_options("program_name");
    BOOST_CHECK(!options.valid());
}

BOOST_AUTO_TEST_SUITE_END()

}
