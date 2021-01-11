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

#ifndef LIBSHRINKLER_LIBSHRINKLER_HPP_INCLUDED
#define LIBSHRINKLER_LIBSHRINKLER_HPP_INCLUDED

#include <cstddef>
#include <iosfwd>
#include <vector>

namespace libshrinkler
{

class shrinkler_parameters
{
public:
    shrinkler_parameters(int preset = 2)
        : m_iterations(1 * preset),
        m_length_margin(1 * preset),
        m_same_length(10 * preset),
        m_effort(100 * preset),
        m_skip_length(1000 * preset),
        m_references(100000) {}

    int iterations() const { return m_iterations; }

    int length_margin() const{ return m_length_margin; }

    int same_length() const { return m_same_length; }

    int effort() const { return m_effort; }

    int skip_length() const { return m_skip_length; }

    int references() const { return m_references; }

private:
    int m_iterations;
    int m_length_margin;
    int m_same_length;
    int m_effort;
    int m_skip_length;
    int m_references;
};

class shrinkler
{
public:
    const shrinkler_parameters& parameters();

    void parameters(const shrinkler_parameters& pack_parameters);

    std::ostream& ostream();

    void os(std::ostream& ostream);

    std::vector<std::byte> compress(const std::vector<std::byte>& input);

private:
    shrinkler_parameters m_parameters;
};

}

#endif
