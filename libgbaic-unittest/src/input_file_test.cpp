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
#include <sstream>
#include <stdexcept>
#include "input_file.hpp"

namespace libgbaic_unittest
{

using std::runtime_error;

BOOST_AUTO_TEST_SUITE(input_file_test)

BOOST_AUTO_TEST_CASE(load_elf_file_does_not_exist)
{
	BOOST_CHECK_EXCEPTION(
		libgbaic::input_file f("non-existing-file.elf"),
		runtime_error,
		[](const auto& e) { BOOST_CHECK_EQUAL("non-existing-file.elf: No such file or directory", e.what()); return true; });
}

BOOST_AUTO_TEST_CASE(load_elf_file_is_invalid)
{
	const char c = 0x42;
	std::stringstream s;
	s.write(&c, 1);

	BOOST_CHECK_EXCEPTION(
		libgbaic::input_file f(s),
		runtime_error,
		[](const auto& e) { BOOST_CHECK_EQUAL("file is not a valid ELF file", e.what()); return true; });
}

BOOST_AUTO_TEST_SUITE_END()

}
