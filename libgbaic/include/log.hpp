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

// Really tiny logging facility. No log levels, no timestamps, one sink: stdout.
// Exactly what gbaic needs to print optional verbose messages, but not more.

#ifndef LIBGBAIC_LOG_HPP
#define LIBGBAIC_LOG_HPP

#include <iostream>
#include <sstream>

namespace libgbaic::log
{

namespace detail
{

class configuration
{
public:
    static void initialize(bool enabled) { m_enabled = enabled; }
    static bool enabled() { return m_enabled; }
private:
    inline static bool m_enabled = false;
};

class log_message
{
public:
    log_message() {}
    ~log_message() { std::cout << m_buffer.str() << std::endl; }
    std::stringstream& buffer() { return m_buffer; }
private:
    log_message(const log_message&) = delete;
    log_message& operator = (const log_message&) = delete;
    std::stringstream m_buffer;
};

}

inline void initialize(bool enabled) { detail::configuration::initialize(enabled); }

inline bool is_enabled() { return detail::configuration::enabled(); }

}

#define LOG                                                     \
    if (!::libgbaic::log::detail::configuration::enabled());    \
        else ::libgbaic::log::detail::log_message().buffer()

#endif
