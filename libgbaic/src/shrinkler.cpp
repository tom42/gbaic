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

#include <cstdio>
#include <cstdlib>
#include "console.hpp"
#include "shrinkler.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4018 4100 4244 4389 4456 4458)
#endif

#include "Pack.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace libgbaic
{

std::vector<unsigned char> shrinkler::compress(const std::vector<unsigned char>& /*input*/)
{
	m_console.out << "Compressing..." << std::endl;

    // TODO: shrinkler compression
    return std::vector<unsigned char>();
}

}

// TODO: port stuff below
/*
* From Shrinkler.cpp:
* 

		printf("Crunching...\n\n");
		RefEdgeFactory edge_factory(references.value);
		DataFile *crunched = orig->crunch(&params, &edge_factory, !no_progress.seen);
		delete orig;
		printf("References considered:%8d\n",  edge_factory.max_edge_count);
		printf("References discarded:%9d\n\n", edge_factory.max_cleaned_edges);

		printf("Saving file %s...\n\n", outfile);
		crunched->save(outfile);

		printf("Final file size: %d\n\n", crunched->size());
		delete crunched;

		if (edge_factory.max_edge_count > references.value) {
			printf("Note: compression may benefit from a larger reference buffer (-r option).\n\n");
		}
*/
