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
#include "console.hpp"
#include "shrinkler.hpp"
#include "test_utilities.hpp"

namespace libgbaic_unittest
{

BOOST_AUTO_TEST_SUITE(shrinkler_test)

BOOST_AUTO_TEST_CASE(shrinkler_test)
{
    libgbaic::shrinkler shrinkler(libgbaic::console(false));
    shrinkler.parameters(libgbaic::shrinkler_parameters(9));

    shrinkler.compress(load_binary_file("lostmarbles.bin"));
    // TODO: compare compressed against reference data
    // TODO: put this test into separate test suite because it leaks
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()

}
