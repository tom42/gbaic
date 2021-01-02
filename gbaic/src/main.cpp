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

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "options.hpp"

#if 0

// TODO: pass this. This has only an effect with a parse function, apparently. And fix the broken english, please.
static const struct argp_option options[] =
{
    { "output", 'o', "FILE", 0, "Output file, default is input file with the extension replaced by .gba" },
    { 0 }
};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    switch (key)
    {
        case 'o':
            return 0;
        default:
            return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = { options, parse_opt, args_doc, doc };

static void parse_command_line(int argc, char *argv[])
{
    // TODO: test code
    auto result = argp_parse(&argp, argc, argv, ARGP_NO_EXIT, nullptr, nullptr);
    std::cout << "argp parse returned " << result;
    if (result)
    {
        std::cout << " (" << strerror(result) << ")";
    }
    std::cout << std::endl;
}
#endif

int main(int argc, char* argv[])
{
    try
    {
        libgbaic::options options(argc, argv);
        if (!options.is_valid())
        {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
