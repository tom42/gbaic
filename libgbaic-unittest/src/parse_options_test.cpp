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
#include <string>
#include <vector>
#include "options.hpp"

namespace libgbaic_unittest
{

using libgbaic::action;
using std::string;
using std::vector;

class fixture
{
public:
    libgbaic::options options;

    action parse_options(const char* command_line)
    {
        options = libgbaic::options();

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

        return libgbaic::parse_options(boost::numeric_cast<int>(argv.size()), argv.data(), options, true);
    }

private:
    static vector<char> to_vector(const string& s)
    {
        return vector<char>(s.c_str(), s.c_str() + s.size() + 1);
    }
};

BOOST_FIXTURE_TEST_SUITE(parse_options_test, fixture)

BOOST_AUTO_TEST_CASE(empty_command_line)
{
    BOOST_CHECK(action::exit_failure == parse_options(""));
}

BOOST_AUTO_TEST_CASE(more_than_one_input_file)
{
    BOOST_CHECK(action::exit_failure == parse_options("file1 file2"));
}

BOOST_AUTO_TEST_CASE(one_input_file)
{
    BOOST_CHECK(action::process == parse_options("file1"));
    BOOST_CHECK_EQUAL("file1", options.input_file());
    BOOST_CHECK_EQUAL("file1.gba", options.output_file());
}

BOOST_AUTO_TEST_CASE(help_option)
{
    BOOST_CHECK(action::exit_success == parse_options("-?"));
    BOOST_CHECK(action::exit_success == parse_options("--help"));
}

BOOST_AUTO_TEST_CASE(version_option)
{
    BOOST_CHECK(action::exit_success == parse_options("-V"));
    BOOST_CHECK(action::exit_success == parse_options("--version"));
}

BOOST_AUTO_TEST_CASE(usage_option)
{
    BOOST_CHECK(action::exit_success == parse_options("--usage"));
}

BOOST_AUTO_TEST_CASE(invalid_option)
{
    BOOST_CHECK(action::exit_failure == parse_options("--invalid-option"));
}

BOOST_AUTO_TEST_CASE(output_file_option_before_input_file)
{
    BOOST_CHECK(action::process == parse_options("-o output input"));
    BOOST_CHECK_EQUAL("input", options.input_file());
    BOOST_CHECK_EQUAL("output", options.output_file());
}

BOOST_AUTO_TEST_CASE(output_file_option_after_input_file)
{
    BOOST_CHECK(action::process == parse_options("input -o output"));
    BOOST_CHECK_EQUAL("input", options.input_file());
    BOOST_CHECK_EQUAL("output", options.output_file());
}

BOOST_AUTO_TEST_CASE(shrinkler_iterations_option)
{
    BOOST_CHECK(action::exit_failure == parse_options("input -i"));
    BOOST_CHECK(action::exit_failure == parse_options("input -i x"));
    BOOST_CHECK(action::exit_failure == parse_options("input -i 0"));
    BOOST_CHECK(action::exit_failure == parse_options("input -i 10"));

    // TODO: test -i1, should set the option to 1
    // TODO: test -i9, should set the option to 9
}

// TODO: test parsing of shrinkler options (too small, min, max, too big)
//       .iterations());
//       .length_margin());
//       .same_length());
//       .effort());
//       .skip_length());
//       .references());
// TODO: TEST preset (too small, min, max, too big)
// TODO: test by default it is using default options?
/*
    // TODO: min/max values for reference
    IntParameter    iterations    ("-i", "--iterations",      1,        9,    1*p, argc, argv, consumed);
    IntParameter    length_margin ("-l", "--length-margin",   0,      100,    1*p, argc, argv, consumed);
    IntParameter    same_length   ("-a", "--same-length",     1,   100000,   10*p, argc, argv, consumed);
    IntParameter    effort        ("-e", "--effort",          0,   100000,  100*p, argc, argv, consumed);
    IntParameter    skip_length   ("-s", "--skip-length",     2,   100000, 1000*p, argc, argv, consumed);
    IntParameter    references    ("-r", "--references",   1000,100000000, 100000, argc, argv, consumed);
*/

BOOST_AUTO_TEST_SUITE_END()

}
