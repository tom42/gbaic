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

#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include "cxxopts.hpp"
#include "options.hpp"
#include "version.hpp"

namespace libgbaic
{

using std::string;
using std::vector;

static_assert(std::is_move_constructible<libgbaic::options>::value, "libgbaic::options is not move constructible");

static const string& get_input_file(const cxxopts::ParseResult& parse_result)
{
    switch (parse_result.count("input-file"))
    {
        case 0:
            throw std::runtime_error("No input file given");
        case 1:
            return parse_result["input-file"].as<vector<string>>().front();
        default:
            throw std::runtime_error("More than one input file given");
    }
}

options parse_options(int argc, char* argv[])
{
    cxxopts::Options options(PROJECT_NAME, "Gameboy Advance Intro Cruncher");

    options.add_options()
        ("h,help", "Print this help")
        ("V,version", "Print program version");
    options.add_options("hidden")
        ("input-file", "Input file", cxxopts::value<vector<string>>());
    options.parse_positional({ "input-file" });
    options.positional_help("<input file>");

    auto result = options.parse(argc, argv);

    libgbaic::options opts;

    if (result.count("help"))
    {
        std::cout << options.help({ "" }) << std::endl;
        opts.should_exit(true);
        return opts;
    }

    if (result.count("version"))
    {
        std::cout << PROJECT_NAME << " " << PROJECT_VERSION << std::endl;
        opts.should_exit(true);
        return opts;
    }

    opts.input_file(get_input_file(result));
    return opts;
}

}

// TODO: redo stuff below using cxxopts
#if 0

options::options(int argc, char* argv[])
{
    static const struct argp_option options[] =
    {
        { "output", 'o', "OUTPUT FILE", 0, "Specify output filename. The default output filename is the name of the input file with the extension replaced by .gba", 0 },
        { 0, 0, 0, 0, 0, 0 }
    };
    static const char doc[] = PROJECT_NAME " - Gameboy Advance Intro Cruncher";
    static const char args_doc[] = "<INPUT FILE>";
    static const struct argp argp = { options, nullptr, args_doc, doc, nullptr, nullptr, nullptr };

    argp_parse(&argp, argc, argv, 0, nullptr, this);
}

}
#endif
