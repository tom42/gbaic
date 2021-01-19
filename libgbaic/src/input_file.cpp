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
#include <elfio/elfio.hpp>
#include <fstream>
#include <stdexcept>
#include "input_file.hpp"

namespace libgbaic
{

input_file::input_file(const std::filesystem::path& path)
{
	// TODO: open the stream as binary. Then load ELF file from stream
	try
	{
		std::ifstream stream(path, std::ios::binary);
		if (!stream)
		{
			// TODO: use strerror_r / strerror_s, no?
			// TODO: unit test?
			throw std::runtime_error(strerror(errno));
		}

		ELFIO::elfio reader;
		if (!reader.load(stream))
		{
			// TODO: unit test?
			throw std::runtime_error("not a valid ELF file");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(path.string() + ": " + e.what());
	}
}

}
