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

#include <boost/test/unit_test.hpp>
#include "options.hpp"

namespace libgbaic_unittest
{

using libgbaic::options;

BOOST_AUTO_TEST_SUITE(options_test)

BOOST_AUTO_TEST_CASE(constructor)
{
    options options;

    BOOST_CHECK_EQUAL("", options.input_file());
    BOOST_CHECK_EQUAL("", options.output_file());
    BOOST_CHECK_EQUAL(false, options.verbose());
}

BOOST_AUTO_TEST_CASE(input_file_sets_output_file_if_not_yet_set)
{
    options options;

    options.input_file("input.elf");

    BOOST_CHECK_EQUAL("input.gba", options.output_file());
}

BOOST_AUTO_TEST_CASE(input_file_does_not_set_output_file_if_already_set)
{
    options options;

    options.output_file("output.gba");
    options.input_file("input.bin");

    BOOST_CHECK_EQUAL("input.bin", options.input_file());
    BOOST_CHECK_EQUAL("output.gba", options.output_file());
}

BOOST_AUTO_TEST_SUITE_END()

}
