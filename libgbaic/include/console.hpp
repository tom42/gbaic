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

#ifndef LIBGBAIC_CONSOLE_HPP_INCLUDED
#define LIBGBAIC_CONSOLE_HPP_INCLUDED

#include <iostream>

namespace libgbaic
{

class console
{
public:
    console() : console(&std::cout, &std::cout) {}

    console(bool out_enabled, bool verbose_enabled) : console(out_enabled ? &std::cout : nullptr, verbose_enabled ? &std::cout : nullptr) {}

    bool out_enabled() const { return m_out != nullptr; }
    bool verbose_enabled() const { return m_verbose != nullptr; }

    std::ostream& out() { return *m_out; }
    std::ostream& verbose() { return *m_verbose; }

private:
    console(std::ostream* out, std::ostream* verbose) : m_out(out), m_verbose(verbose) {}

    std::ostream* const m_out;
    std::ostream* const m_verbose;
};

}

#define CONSOLE_OUT(console)            \
    if (!(console).out_enabled());      \
        else (console).out()

#define CONSOLE_VERBOSE(console)        \
    if (!(console).verbose_enabled());  \
        else (console).verbose()

#endif
