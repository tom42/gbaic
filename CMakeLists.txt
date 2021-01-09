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

cmake_minimum_required(VERSION 3.17.3)
project(gbaic VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_MAIN_PROJECT TRUE)
else()
  set(IS_MAIN_PROJECT FALSE)
endif()

if(IS_MAIN_PROJECT)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  include(CTest)
endif()


################################################################################
# Find software packages
################################################################################

find_package(Boost 1.74 COMPONENTS unit_test_framework)
if(Boost_FOUND)
  message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
  message(STATUS "Boost library directories: ${Boost_LIBRARY_DIRS}")
endif()


################################################################################
# Subdirectories with 3rd party code.
################################################################################

add_subdirectory(3rdparty)


################################################################################
# Subdirectories with own code.
################################################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(libshrinkler)
add_subdirectory(libgbaic)
add_subdirectory(gbaic)

if(Boost_FOUND)
  add_subdirectory(libgbaic-unittest)
else()
  message(STATUS "Boost Unit Test Framework not found. Unit tests will not be built.")
endif()
