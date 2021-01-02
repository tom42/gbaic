// MIT License
//
// gbaic - Gameboy Advance Intro Cruncher
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

#include "argp.h"
#include "options.hpp"
#include "version.hpp"

extern "C"
{
    const char* argp_program_version = PROJECT_NAME " " PROJECT_VERSION;
    const char* argp_program_bug_address = "https://github.com/tom42/gbaic/issues";
}

namespace libgbaic
{

options::options(int argc, char* argv[])
{
    static const char doc[] = PROJECT_NAME " - Gameboy Advance Intro Cruncher";
    static const struct argp argp = { nullptr, nullptr, nullptr, doc, nullptr, nullptr, nullptr };

    argp_parse(&argp, argc, argv, 0, nullptr, this);
}

}
