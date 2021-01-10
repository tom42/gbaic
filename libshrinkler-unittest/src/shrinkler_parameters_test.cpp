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
#include "libshrinkler.hpp"

namespace libshrinkler_unittest
{

BOOST_AUTO_TEST_SUITE(shrinkler_parameters_test)

BOOST_AUTO_TEST_CASE(constructor)
{
    libshrinkler::shrinkler_parameters parameters;

    BOOST_CHECK_EQUAL(2, parameters.iterations);
    BOOST_CHECK_EQUAL(2, parameters.length_margin);
    BOOST_CHECK_EQUAL(20, parameters.same_length);
    BOOST_CHECK_EQUAL(200, parameters.effort);
    BOOST_CHECK_EQUAL(2000, parameters.skip_length);
    BOOST_CHECK_EQUAL(100000, parameters.references);
}

BOOST_AUTO_TEST_SUITE_END()

}
