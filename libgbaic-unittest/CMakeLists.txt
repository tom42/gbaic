# MIT License
#
# gbaic: Gameboy Advance Intro Cruncher
# Copyright (c) 2020 Thomas Mathys
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(LIBGBAIC_UNITTEST_TESTDATA_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testdata")
configure_file(libgbaic_unittest_config.hpp.in libgbaic_unittest_config.hpp)

set(
  SOURCES
  src/input_file_test.cpp
  src/main.cpp
  src/options_test.cpp
  src/parse_options_test.cpp
  src/shrinkler_parameters_test.cpp
  src/shrinkler_test.cpp
  src/test_utilities.cpp
  src/test_utilities.hpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_executable(libgbaic-unittest ${SOURCES})

target_include_directories(
  libgbaic-unittest
  PRIVATE
  "${Boost_INCLUDE_DIRS}"
  "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(libgbaic-unittest PRIVATE libgbaic)

add_test(NAME libgbaic-unittest COMMAND libgbaic-unittest)
