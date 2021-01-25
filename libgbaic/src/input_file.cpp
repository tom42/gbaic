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

// Some quotes from the ELF specifications:
//
// Segment type PT_LOAD:
//
// "The array element specifies a loadable segment, described by p_filesz and p_memsz.
// The bytes from the file are mapped to the beginning of the memory segment.
// If the segment's memory size (p_memsz) is larger than the file size (p_filesz),
// the ``extra'' bytes are defined to hold the value 0 and to follow the segment's
// initialized area. The file size may not be larger than the memory size.
// Loadable segment entries in the program header table appear in ascending order,
// sorted on the p_vaddr member."
//
// p_align field of segment header:
//
// "Loadable process segments must have congruent values for p_vaddr and p_offset,
// modulo the page size. This member gives the value to which the segments are aligned
// in memory and in the file.
// Values 0 and 1 mean no alignment is required. Otherwise, p_align should be a positive,
// integral power of 2, and p_vaddr should equal p_offset, modulo p_align."

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
using ELFIO::segment;
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

static void throw_if_load_segment_is_out_of_order(segment* last, segment* current)
{
    // Required by ELF specifications.
    if (last && (current->get_virtual_address() < last->get_virtual_address()))
    {
        throw runtime_error("invalid ELF file. LOAD segment headers are not sorted by ascending virtual address");
    }
}

static void throw_if_invalid_load_segment(segment* seg)
{
    // Not sure this is a problem. Refuse to process such a file until we know.
    if (seg->get_virtual_address() != seg->get_physical_address())
    {
        throw runtime_error("file contains LOAD segment whose virtual and physical address differ. Don't know how to handle that");
    }

    // Required by ELF specifications.
    if (seg->get_file_size() > seg->get_memory_size())
    {
        throw runtime_error("invalid ELF file. Found LOAD segment whose file size is larger than its memory size");
    }

    // Not sure this could be a problem, but for the time being require
    // segment alignment to be according to ELF specifications.
    const auto align = seg->get_align();
    if (align > 1)
    {
        if ((align & (align - 1)) != 0)
        {
            throw runtime_error("invalid ELF file. Found LOAD segment whose alignment is not a power of 2");
        }

        if ((seg->get_offset() % align) != (seg->get_virtual_address() % align))
        {
            throw runtime_error("invalid ELF file. Found LOAD segment where (offset % align) != (virtual address % align)");
        }
    }
}

static void verify_load_segment(segment* last, segment* current)
{
    throw_if_load_segment_is_out_of_order(last, current);
    throw_if_invalid_load_segment(current);
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
    convert_to_binary(reader);
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

void input_file::convert_to_binary(elfio& reader)
{
    // TODO: Do we initially check whether there are any program headers?
    //       Or do we simply do all the processing and fail if there is no data left?
    //       Initial checks might be
    //       * Are there any LOAD headers?
    //       * Are the LOAD headers correctly ordered?

    // TODO: go through all the headers
    //       * If it is a LOAD header
    //         * Add data to binary data (or not, if not needed)
    //         * Do that for each.
    //         * Profit
    segment* last = nullptr;
    const Elf_Half nheaders = reader.segments.size();
    for (Elf_Half i = 0; i < nheaders; ++i)
    {
        segment* current = reader.segments[i];
        if (current->get_type() == PT_LOAD)
        {
            verify_load_segment(last, current);
            last = current;
            // TODO:
            // * Headers should not overlap
        }
    }
}

void input_file::verbose_log(const string& s)
{
    // TODO: make it possible to disable output
    std::cout << s << std::endl;
}

}
