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

#include <cstdlib>
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
    parser(options& options, bool silent) : m_options(options), m_silent(silent), m_action(action::process), m_inputfile_seen(false) {}

    error_t parse_opt(int key, char* arg, argp_state* state)
    {
        switch (key)
        {
            case 'o':
                m_options.output_file(arg);
                return 0;
            case 'a':
                return parse_int("same length count", arg, 1, 100000, state, m_options.shrinkler_parameters().same_length);
            case 'e':
                return parse_int("effort", arg, 0, 100000, state, m_options.shrinkler_parameters().effort);
            case 'i':
                return parse_int("number of iterations", arg, 1, 9, state, m_options.shrinkler_parameters().iterations);
            case 'l':
                return parse_int("length margin", arg, 0, 100, state, m_options.shrinkler_parameters().length_margin);
            case 'r':
                return parse_int("number of references", arg, 1000, 100000000, state, m_options.shrinkler_parameters().references);
            case 's':
                return parse_int("skip length", arg, 2, 100000, state, m_options.shrinkler_parameters().skip_length);
            case '?':
                argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
                stop_parsing_and_exit(state);
                return 0;
            case 'V':
                print_version();
                stop_parsing_and_exit(state);
                return 0;
            case option::usage:
                argp_state_help(state, stdout, ARGP_HELP_USAGE);
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
                    argp_error(state, "more than one input file given");
                    return EINVAL;
                }
            case ARGP_KEY_NO_ARGS:
                if (m_action != action::exit_success)
                {
                    argp_error(state, "no input file given");
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

    void print_version()
    {
        if (!m_silent)
        {
            std::cout << PROJECT_NAME << " " << PROJECT_VERSION << std::endl;
        }
    }

    static int parse_int(const char* value_description, const char* s, int min, int max, const argp_state* state, int& parsed_int)
    {
        char* end;
        auto value = strtol(s, &end, 10);

        if ((*end) || (value < min) || (value > max))
        {
            // Exit code (EXIT_FAILURE) is not really used here since we don't allow argp to exit the program.
            // But if we did then that's what we wanted to have as exit code.
            argp_failure(state, EXIT_FAILURE, 0, "invalid %s: %s", value_description, s);
            return EINVAL;
        }

        parsed_int = value;
        return 0;
    }

    options& m_options;
    const bool m_silent;
    libgbaic::action m_action;
    bool m_inputfile_seen;
};

static error_t parse_opt(int key, char* arg, argp_state* state)
{
    // TODO: catch all exceptions here and somehow abort
    parser& p = *static_cast<parser*>(state->input);
    return p.parse_opt(key, arg, state);
}

action parse_options(int argc, char* argv[], options& options, bool silent)
{
    static const char doc[] =
        PROJECT_NAME " - Gameboy Advance Intro Cruncher by Tom/Vantage\n"
        "Uses Shrinkler compression by Blueberry\n"
        "https://github.com/tom42/gbaic";
    static const char args_doc[] = "FILE";

    static const argp_option argp_options[] =
    {
        { 0, 0, 0, 0, "General options:", 0 },
        { "output-file", 'o', "FILE", 0, "Specify output filename. The default output filename is the input filename with the extension replaced by .gba", 0 },

        // Shrinkler compression options
        { 0, 0, 0, 0, "Shrinkler compression options (default values in parentheses):", 0 },
        { "same-length", 'a', "N", 0, "Number of matches of the same length to consider (20)", 0 },
        { "effort", 'e', "N", 0, "Perseverance in finding multiple matches (200)", 0 },
        { "iterations", 'i', "N", 0, "Number of iterations for the compression (2)", 0 },
        { "length-margin", 'l', "N", 0, "Number of shorter matches considered for each match (2)", 0 },
        { "preset", 'p', "PRESET", 0, "Preset for all compression options except --references (1..9, default 2)", 0 },
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

    parser parser(options, silent);

    unsigned int flags = ARGP_NO_EXIT | ARGP_NO_HELP;
    if (silent)
    {
        flags |= ARGP_NO_ERRS;
    }

    if (argp_parse(&argp, argc, argv, flags, 0, &parser))
    {
        return action::exit_failure;
    }

    return parser.action();
}

}
