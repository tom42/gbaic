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
            throw cxxopts::OptionParseException("No input file given");
        case 1:
            return parse_result["input-file"].as<vector<string>>().front();
        default:
            throw cxxopts::OptionParseException("More than one input file given");
    }
}

options parse_options(std::ostream& os, int argc, char* argv[])
{
    try
    {
        cxxopts::Options options(
            PROJECT_NAME,
            PROJECT_NAME " - Gameboy Advance Intro Cruncher by Tom/Vantage\n"
            "Shrinkler compressor by Blueberry");

        options.add_options()
            ("h,help", "Print this help")
            ("o,output-file", "Specify output filename. The default output filename is the input filename with the extension replaced by .gba", cxxopts::value<string>())
            ("V,version", "Print program version");
        options.add_options("hidden")
            ("input-file", "Input file", cxxopts::value<vector<string>>());
        options.parse_positional({ "input-file" });
        options.positional_help("<input file>");

        auto result = options.parse(argc, argv);

        libgbaic::options opts;

        if (result.count("help"))
        {
            os << options.help({ "" }) << std::endl;
            opts.should_exit(true);
            return opts;
        }

        if (result.count("version"))
        {
            os << PROJECT_NAME << " " << PROJECT_VERSION << std::endl;
            opts.should_exit(true);
            return opts;
        }

        opts.input_file(get_input_file(result));

        if (result.count("output-file"))
        {
            opts.output_file(result["output-file"].as<string>());
        }

        return opts;
    }
    catch (const cxxopts::OptionParseException& e)
    {
        throw std::runtime_error(string(e.what()) + "\nTry '" PROJECT_NAME " --help' for more information");
    }
}

}
