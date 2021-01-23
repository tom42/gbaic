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

#define _CRT_SECURE_NO_WARNINGS // TODO: really? I mean...we should be using strerror_r or strerror_s, no?
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include "elfio/elfio.hpp"
#include "fmt/core.h"
#include "input_file.hpp"

namespace libgbaic
{

using ELFIO::elfio;
using ELFIO::Elf_Half;
using ELFIO::Elf_Word;
using fmt::format;
using std::string;
using std::runtime_error;

static string segment_type_to_string(Elf_Word type)
{
    struct table_entry
    {
        Elf_Word type;
        const char* string;
    };
    static const table_entry table[] =
    {
        { PT_NULL,    "NULL" },
        { PT_LOAD,    "LOAD" },
        { PT_DYNAMIC, "DYNAMIC" },
        { PT_INTERP,  "INTERP" },
        { PT_NOTE,    "NOTE" },
        { PT_SHLIB,   "SHLIB" },
        { PT_PHDR,    "PHDR" },
        { PT_TLS,     "TLS" },
        { PT_LOOS,    "LOOS" },
        { PT_HIOS,    "HIOS" },
        { PT_LOPROC,  "LOPROC" },
        { PT_HIPROC,  "HIPROC" }
    };
    static const size_t table_length = sizeof(table) / sizeof(table[0]);

    for (size_t i = 0; i < table_length; ++i)
    {
        if (table[i].type == type)
        {
            return table[i].string;
        }
    }

    return fmt::format("{:#010x}", type);
}

static string segment_flags_to_string(Elf_Word flags)
{
    static const char* const table[] = { "", "X", "W", "WX", "R", "RX", "RW", "RWX" };
    static const size_t table_length = sizeof(table) / sizeof(table[0]);

    if (flags >= table_length)
    {
        return fmt::format("{:#x}", flags);
    }

    return table[flags];
}

static void open_elf(elfio& reader, std::istream& stream)
{
    if (!reader.load(stream))
    {
        throw runtime_error("file is not a valid ELF file");
    }
}

static void check_executable_type(elfio& reader)
{
    if ((reader.get_class() != ELFCLASS32) ||
        (reader.get_encoding() != ELFDATA2LSB) ||
        (reader.get_type() != ET_EXEC) ||
        (reader.get_machine() != EM_ARM))
    {
        throw runtime_error("file is not a 32-bit little endian ARM executable ELF file");
    }
}

static void check_elf_version(elfio& reader)
{
    const auto expected_elf_version = 1;

    auto ei_version = reader.get_elf_version();
    if (ei_version != expected_elf_version)
    {
        throw runtime_error(format("unknown ELF format version {}. Expected {}", ei_version, expected_elf_version));
    }
}

static void check_os_abi(elfio& reader)
{
    const auto expected_abi = ELFOSABI_NONE;

    auto ei_osabi = reader.get_os_abi();
    if (ei_osabi != expected_abi)
    {
        throw runtime_error(format("unknown ELF OS ABI {}. Expected none ({})", ei_osabi, expected_abi));
    }
}

static void check_abi_version(elfio& reader)
{
    const auto expected_abi_version = 0;

    auto ei_abiversion = reader.get_abi_version();
    if (ei_abiversion != expected_abi_version)
    {
        throw runtime_error(format("unknown ABI version {}. Expected {}", ei_abiversion, expected_abi_version));
    }
}

static void check_object_file_version(elfio& reader)
{
    const auto expected_object_file_version = 1;

    auto e_version = reader.get_version();
    if (e_version != expected_object_file_version)
    {
        throw runtime_error(format("unknown object file version {}. Expected {}", e_version, expected_object_file_version));
    }
}

static void check_header(elfio& reader)
{
    check_executable_type(reader);
    check_elf_version(reader);

    // Not sure these matter. Checking them to be on the safe side.
    check_os_abi(reader);
    check_abi_version(reader);
    check_object_file_version(reader);

    // TODO: do consider having some sort of verbose mode, since things ARE going to fail
    // TODO: introduce some verbose logging. Until we know better what we need just put this into some function, no?)
    // TODO: store this, we're going to need it.
    //       Do we limit the entry points we are going to accept? Well perhaps, but not in this class.
    //       How do we find the load address, anyway?
}

input_file::input_file(const std::filesystem::path& path)
{
    try
    {
        verbose_log(fmt::format("Loading: {}", path.string()));
        std::ifstream stream(path, std::ios::binary);
        if (!stream)
        {
            // TODO: use strerror_r / strerror_s, no?
            throw runtime_error(strerror(errno));
        }

        load_elf(stream);
    }
    catch (const std::exception& e)
    {
        throw runtime_error(path.string() + ": " + e.what());
    }
}

input_file::input_file(std::istream& stream)
{
    load_elf(stream);
}

void input_file::load_elf(std::istream& stream)
{
    elfio reader;

    open_elf(reader, stream);
    check_header(reader);
    read_entry(reader);
    log_program_headers(reader);
    // TODO: log entry point and load address at the very end
}

void input_file::read_entry(elfio& reader)
{
    m_entry = reader.get_entry();
    verbose_log(fmt::format("Entry: {:#x}", m_entry));
}

void input_file::log_program_headers(elfio& reader)
{
    // TODO: exit if verbose logging is disabled.

    Elf_Half nheaders = reader.segments.size();
    if (nheaders == 0)
    {
        // TODO: when just logging this is OK, but when actually converting to binary we'll probably fail if this is the case, no?
        verbose_log("File has no program headers");
        return;
    }

    verbose_log("Program headers");
    verbose_log(fmt::format(" {:10} {:7} {:10} {:10} {:7} {:7} {:7} {:3}",
        "Type",
        "Offset",
        "VirtAddr",
        "PhysAddr",
        "FileSiz",
        "MemSiz",
        "Align",
        "Flg"));

    for (Elf_Half i = 0; i < nheaders; ++i)
    {
        const auto& s = *reader.segments[i];
        verbose_log(fmt::format(" {:10} {:#07x} {:#010x} {:#010x} {:#07x} {:#07x} {:#07x} {}",
            segment_type_to_string(s.get_type()),
            s.get_offset(),
            s.get_virtual_address(),
            s.get_physical_address(),
            s.get_file_size(),
            s.get_memory_size(),
            s.get_align(),
            segment_flags_to_string(s.get_flags())));
    }
}

void input_file::verbose_log(const string& s)
{
    // TODO: make it possible to disable output
    std::cout << s << std::endl;
}

}
