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
#include <vector>
#include "cxxopts.hpp"
#include "options.hpp"
#include "version.hpp"

namespace libgbaic
{

const options parse_options(int argc, char* argv[])
{
    cxxopts::Options options(PROJECT_NAME, "Gameboy Advance Intro Cruncher");

    options.add_options()
        ("h,help", "Print this help")
        ("V,version", "Print program version")
        // TODO: test thingies, remove
        ("i,int", "Specify an int", cxxopts::value<int>())
        ("b,bool", "Specify a bool", cxxopts::value<bool>())
        ("c,cool", "Specify a cool");

    options.add_options("hidden")
        ("input-file", "Input file", cxxopts::value<std::vector<std::string>>());

    options.parse_positional({ "input-file" });

    auto result = options.parse(argc, argv);

    // TODO: verify options

    // TODO: print help, version, etc (only one, I'd say? or all of them?)
    if (result.count("help"))
    {
        std::cout << options.help({ "" }) << std::endl;
    }

    // TODO: test code: dump positional args
    if (result.count("input-file"))
    {
        std::cout << "Positional = {";
        auto& v = result["input-file"].as<std::vector<std::string>>();
        for (const auto& s : v) {
            std::cout << s << ", ";
        }
        std::cout << "}" << std::endl;
    }

    throw std::runtime_error("yikes");
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
