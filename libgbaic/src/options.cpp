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

#include <type_traits>
#include "argp.h"
#include "options.hpp"
#include "version.hpp"

extern "C"
{
    const char* argp_program_version = PROJECT_NAME " " PROJECT_VERSION;
    const char* argp_program_bug_address = "TODO: do we want this?";
}

namespace libgbaic
{

static_assert(std::is_move_constructible<libgbaic::options>::value, "libgbaic::options is not move constructible");

// TODO: somehow, stop this from exiting. On any path.
// TODO: remove os argument
// TODO: unit test for
//       * --version
//       * --usage
//       * --help
options parse_options(std::ostream& /*os*/, int argc, char* argv[])
{
    static const char doc[] =
        PROJECT_NAME " - Gameboy Advance Intro Cruncher by Tom/Vantage\n"
        "Shrinkler compressor by Blueberry";
    static const char args_doc[] = "FILE";

    static const struct argp_option options[] =
    {
        // TODO: try intercepting --version, --help, --usage, if possible. If not, reimplement them.
        { 0, 0, 0, 0, 0 }
    };

    static const struct argp argp = { options, 0, args_doc, doc, 0, 0, 0 };

    libgbaic::options result;
    argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, 0);
    return result;
}


}

// TODO: redo stuff below
#if 0
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

using cxxopts::value;
using std::string;
using std::vector;


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
        options.add_options()
            ("o,output-file", "Specify output filename. The default output filename is the input filename with the extension replaced by .gba", value<string>(), "FILE");

        options.add_options("Shrinkler compression")
            ("i,iterations", "Number of iterations for the compression (2)", value<int>(), "N")
            ("l,length-margin", "Number of shorter matches considered for each match (2)", value<int>(), "N")
            ("a,same-length", "Number of matches of the same length to consider (20)", value<int>(), "N")
            ("e,effort", "Perseverance in finding multiple matches (200)", value<int>(), "N")
            ("s,skip-length", "Minimum match length to accept greedily (2000)", value<int>(), "N")
            ("r,references", "Number of reference edges to keep in memory (100000)", value<int>(), "N")
            ("p,preset", "Preset for all compression options except --references (1..9, default 2)", value<int>(), "N");

        options.add_options("Help")
            ("h,help", "Print this help")
            ("V,version", "Print program version");

        options.add_options("hidden")
            ("input-file", "Input file", value<vector<string>>());

        options.parse_positional({ "input-file" });
        options.positional_help("<INPUT FILE>");

        auto result = options.parse(argc, argv);

        libgbaic::options opts;

        if (result.count("help"))
        {
            os << options.help({ "", "Shrinkler compression", "Help" }) << std::endl;
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
#endif
