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

#ifndef LIBGBAIC_INPUT_FILE_HPP_INCLUDED
#define LIBGBAIC_INPUT_FILE_HPP_INCLUDED

#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

namespace ELFIO
{

class elfio;
class segment;

}

namespace libgbaic
{

class console;

class input_file
{
public:
    input_file(console& c) : m_console(c) {}

    void load(const std::filesystem::path& path);

    void load(std::istream& stream);

    uint_fast64_t entry() const { return m_entry; }

    uint_fast64_t load_address() const { return m_load_address; }

    const std::vector<unsigned char>& data() const { return m_data; }

private:
    void load_elf(std::istream& stream);
    void read_entry(ELFIO::elfio& reader);
    void log_program_headers(ELFIO::elfio& reader);
    void convert_to_binary(ELFIO::elfio& reader);

    console& m_console;
    uint_fast64_t m_entry = 0;
    uint_fast64_t m_load_address = 0;
    std::vector<unsigned char> m_data;
};

}

#endif
