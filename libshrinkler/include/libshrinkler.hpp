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
        : shrinkler_parameters(1 * preset, 1 * preset, 10 * preset, 100 * preset, 1000 * preset, 100000) {}

    shrinkler_parameters(int iterations, int length_margin, int same_length, int effort, int skip_length, int references)
        : iterations(iterations),
        length_margin(length_margin),
        same_length(same_length),
        effort(effort),
        skip_length(skip_length),
        references(references) {}

    int iterations;
    int length_margin;
    int same_length;
    int effort;
    int skip_length;
    int references;
};

class shrinkler
{
public:
    const shrinkler_parameters& parameters() const { return m_parameters; }

    void parameters(const shrinkler_parameters& parameters) { m_parameters = parameters; }

    std::vector<unsigned char> compress(const std::vector<unsigned char>& input);

private:
    shrinkler_parameters m_parameters;
};

}

#endif
