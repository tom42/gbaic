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

#define _CRT_SECURE_NO_WARNINGS // TODO: really? I mean...we should be using strerror_r or strerror_s, no?
#include <cstring>
#include <elfio/elfio.hpp>	// TODO: <elfio/elfio.hpp> or "elfio/elfio.hpp"?
#include <fmt/core.h>	    // TODO: <> or ""?
#include <fstream>
#include <stdexcept>
#include "input_file.hpp"

namespace libgbaic
{

using ELFIO::elfio;
using fmt::format;
using std::runtime_error;

static void open_elf(elfio& reader, std::istream& stream)
{
	if (!reader.load(stream))
	{
		throw runtime_error("file is not a valid ELF file");
	}
}

static void check_bitness_and_endianness(elfio& reader)
{
	if ((reader.get_class() != ELFCLASS32) || (reader.get_encoding() != ELFDATA2LSB))
	{
		throw runtime_error("file is not a 32-bit little endian ELF file");
	}
}

static void check_elf_version(elfio& reader)
{
	auto ei_version = reader.get_elf_version();
	if (ei_version != 1)
	{
		throw runtime_error(format("unknown ELF format version {}", ei_version));
	}
}

static void check_os_abi(elfio& reader)
{
	auto ei_osabi = reader.get_os_abi();
	if (ei_osabi != ELFOSABI_NONE)
	{
		throw runtime_error(format("unknown ELF OS ABI {0}. Expected none ({1})", ei_osabi, ELFOSABI_NONE));
	}
}

static void check_header(elfio& reader)
{
	check_bitness_and_endianness(reader);
	check_elf_version(reader);

	// Not sure the OS ABI matters. Checking it to be on the safe side for the time being.
	check_os_abi(reader);

	// TODO: check elf header. Probably we want to check (or perhaps not all of them, need to check):
	// * EI_ABIVERSION										(would't know its meaning. don't check)																(OR expect it to be 0)
	// * e_type												MUST BE ET_EXEC (2)
	// * e_machine											MUST BE 0x28 (ARM)
	// * e_version											MUST BE 0
	// TODO: testcode: CHECK THESE FIELDS
	std::cout << "EI_ABIVERSION: " << ((int)reader.get_abi_version()) << std::endl;
	std::cout << "e_type:        " << reader.get_type() << std::endl;
	std::cout << "e_machine:     " << reader.get_machine() << std::endl;
	std::cout << "e_version:     " << reader.get_version() << std::endl;
	// TODO: do not use std::hex, since it is global.
	// TODO: do consider having some sort of verbose mode, since things ARE going to fail
	std::cout << "entry:         " << std::hex << reader.get_entry() << std::dec << std::endl;
}

input_file::input_file(const std::filesystem::path& path)
{
	try
	{
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
}

}
