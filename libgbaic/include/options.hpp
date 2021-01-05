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

#ifndef LIBGBAIC_OPTIONS_HPP_INCLUDED
#define LIBGBAIC_OPTIONS_HPP_INCLUDED

#include <string>

namespace libgbaic
{

class options
{
public:
    options() : m_should_exit(false) {}

    bool should_exit() const { return m_should_exit; }

    void should_exit(bool should_exit) { m_should_exit = should_exit; }

    const std::string& input_file() const { return m_input_file; }

    void input_file(const std::string& input_file) { m_input_file = input_file; }

    const std::string& output_file() const { return m_output_file; }

    void output_file(const std::string& output_file) { m_output_file = output_file; }

private:
    bool m_should_exit;
    std::string m_input_file;
    std::string m_output_file;
};

options parse_options(int argc, char* argv[]);

}

#endif
