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

configure_file(version.hpp.in version.hpp)

set(
  SOURCES
  include/console.hpp
  include/input_file.hpp
  include/options.hpp
  include/log.hpp
  include/shrinkler.hpp
  src/input_file.cpp
  src/options.cpp
  src/shrinkler.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_library(libgbaic ${SOURCES})

target_include_directories(
  libgbaic
  PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/3rdparty/shrinkler/cruncher"
  "${PROJECT_SOURCE_DIR}/3rdparty/shrinkler/decrunchers_bin"
  PUBLIC
  include)

target_link_libraries(libgbaic PRIVATE argp-standalone elfio fmt)
