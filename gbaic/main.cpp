// MIT License
//
// gbaic - Gameboy Advance Intro Cruncher
// Copyright(c) 2020 Thomas Mathys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstdlib>
#include <iostream>
#include <stdexcept>

// TODO: this is only here for testing
#include <argp.h>

// TODO: defining this gives us the --version switch. We'll want to have that
const char *argp_program_version = "gbaic 0.0.1";

// TODO: this gives us the "report bugs to" blurb. Thing is, I'd rather have this as issues on github,
//       but that requires some hacking since argp insists in printing "Report bugs to <argp_program_bug_address>"
const char *argp_program_bug_address = "/dev/null";

// TODO: this gives us the program documentation. We might get away without, no?
static const char doc[] = "This program compresses stuff";

// TODO: arguments documentation. This string is just printed, it has no meaning for argp (e.g. # of arguments)
static const char args_doc[] = "<FILE>";

// TODO: pass this. This has only an effect with a parse function, apparently.
static const struct argp_option options[] =
{
    { "output", 'o', "FILE", 0 }
};

static struct argp argp = { nullptr, nullptr, args_doc, doc };

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

int main(int argc, char *argv[])
{
    try
    {
        parse_command_line(argc, argv);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
