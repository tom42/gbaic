// MIT License7
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
#include <type_traits>
#include "argp.h"
#include "options.hpp"
#include "version.hpp"

namespace libgbaic
{

static_assert(std::is_move_constructible<libgbaic::options>::value, "libgbaic::options is not move constructible");

enum option
{
    first = 256,
    usage
};

class parser
{
public:
    parser(options& options) : m_options(options), m_action(action::process), m_inputfile_seen(false) {}

    error_t parse_opt(int key, char* arg, argp_state* state)
    {
        switch (key)
        {
            case 'o':
                m_options.output_file(arg);
                return 0;
            case '?':
                argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
                stop_parsing_and_exit(state);
                return 0;
            case 'V':
                print_version();
                stop_parsing_and_exit(state);
                return 0;
            case option::usage:
                argp_state_help(state, stdout, ARGP_HELP_STD_USAGE);
                stop_parsing_and_exit(state);
                return 0;
            case ARGP_KEY_ARG:
                if (!m_inputfile_seen)
                {
                    m_inputfile_seen = true;
                    m_options.input_file(arg);
                    return 0;
                }
                else
                {
                    argp_error(state, "More than one input file given");
                    return EINVAL;
                }
            case ARGP_KEY_NO_ARGS:
                if (m_action != action::exit_success)
                {
                    argp_error(state, "No input file given");
                    return EINVAL;
                }
                else
                {
                    return ARGP_ERR_UNKNOWN;
                }
            default:
                return ARGP_ERR_UNKNOWN;
        }
    }

    action action() const { return m_action; }

private:
    void stop_parsing_and_exit(argp_state* state)
    {
        state->next = state->argc;
        m_action = action::exit_success;
    }

    static void print_version()
    {
        std::cout << PROJECT_NAME << " " << PROJECT_VERSION << std::endl;
    }

    options& m_options;
    libgbaic::action m_action;
    bool m_inputfile_seen;
};

static error_t parse_opt(int key, char* arg, argp_state* state)
{
    // TODO: catch all exceptions here and somehow abort
    parser& p = *static_cast<parser*>(state->input);
    return p.parse_opt(key, arg, state);
}

action parse_options(int argc, char* argv[], options& options)
{
    static const char doc[] =
        PROJECT_NAME " - Gameboy Advance Intro Cruncher by Tom/Vantage\n"
        "Shrinkler compressor by Blueberry";
    static const char args_doc[] = "FILE";

    static const argp_option argp_options[] =
    {
        { 0, 0, 0, 0, "General options:", 0 },
        { "output-file", 'o', "FILE", 0, "Specify output filename. The default output filename is the input filename with the extension replaced by .gba", 0 },

        // TODO: somehow emulate the -1 .. -9 options.
        // TODO: parse them options (new test)
        // TODO: can we somehow disable option sorting?
        /*
            ("p,preset", "Preset for all compression options except --references (1..9, default 2)", value<int>(), "N");
        */

        { 0, 0, 0, 0, "Shrinkler compression options:", 0 },
        { "same-length", 'a', "N", 0, "Number of matches of the same length to consider (20)", 0 },
        { "effort", 'e', "N", 0, "Perseverance in finding multiple matches (200)", 0 },
        { "iterations", 'i', "N", 0, "Number of iterations for the compression (2)", 0 },
        { "length-margin", 'l', "N", 0, "Number of shorter matches considered for each match (2)", 0 },
        { "references", 'r', "N", 0, "Number of reference edges to keep in memory (100000)", 0 },
        { "skip-length", 's', "N", 0, "Minimum match length to accept greedily (2000)", 0 },

        // argp always forces "help" and "version" into group -1, but not "usage".
        // But we want "usage" to be there too, so we explicitly specify -1 for "help".
        // That way, "version" and "usage" will inherit it.
        { "help", '?', 0, 0, "Give this help list", -1 },
        { "version", 'V', 0, 0, "Print program version", 0},
        { "usage", option::usage, 0, 0, "Give a short usage message", 0 },
        { 0, 0, 0, 0, 0, 0 }
    };

    static const argp argp = { argp_options, parse_opt, args_doc, doc, 0, 0, 0 };

    parser parser(options);
    if (argp_parse(&argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, &parser))
    {
        return action::exit_failure;
    }

    return parser.action();
}

}

// TODO: redo all of this shite below
#if 0

extern "C"
{
    const char* argp_program_bug_address = "TODO: do we want this?";
}

// TODO: redo stuff below
#if 0

namespace libgbaic
{

options parse_options(std::ostream& os, int argc, char* argv[])
{
    try
    {

        options.add_options("Shrinkler compression")

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
#endif
