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

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "options.hpp"

int main(int argc, char* argv[])
{
    try
    {
        libgbaic::options options;
        auto action = libgbaic::parse_options(argc, argv, options, false);
        switch (action)
        {
            case libgbaic::action::exit_failure:
                return EXIT_FAILURE;
            case libgbaic::action::exit_success:
                return EXIT_SUCCESS;
            case libgbaic::action::process:
                // TODO: process stuff
                //       * Load input file (bin or elf)
                //       * Compress it (shrinkler or LZSS+Huffman)
                //       * Write output file (should automatically determine which one is the smallest)
                // TODO: test code: dump options
                std::cout << "input:         " << options.input_file() << std::endl;
                std::cout << "output:        " << options.output_file() << std::endl;
                std::cout << "iterations:    " << options.shrinkler_parameters().iterations << std::endl;
                std::cout << "length_margin: " << options.shrinkler_parameters().length_margin << std::endl;
                return EXIT_SUCCESS;
            default:
                throw std::runtime_error("Unknown action returned by command line parser");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
