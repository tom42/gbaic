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

// TODO: do we need pointers all over the place here?
static std::vector<uint32_t> compress(PackParams* /*params*/, RefEdgeFactory* /*edge_factory*/, bool /*show_progress*/)
{
    throw std::runtime_error("yikes");

    // TODO: port stuff below (DataFile::compress)
/*
        vector<unsigned> pack_buffer;
        RangeCoder *range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);

        // Print compression status header
        const char *ordinals[] = { "st", "nd", "rd", "th" };
        printf("Original");
        for (int p = 1 ; p <= params->iterations ; p++) {
            printf("  After %d%s pass", p, ordinals[min(p,4)-1]);
        }
        printf("\n");

        // Crunch the data
        range_coder->reset();
        packData(&data[0], data.size(), 0, params, range_coder, edge_factory, show_progress);
        range_coder->finish();
        printf("\n\n");
        fflush(stdout);

        return pack_buffer;
*/
}

// TODO: do we need pointers all over the place here (already on the args)
static void crunch(PackParams* params, RefEdgeFactory* edge_factory, bool show_progress)
{
    vector<unsigned> pack_buffer = compress(params, edge_factory, show_progress);

    // TODO: port stuff below (DataFile::crunch)
/*
        int margin = verify(pack_buffer);

        printf("Minimum safety margin for overlapped decrunching: %d\n\n", margin);

        DataFile *ef = new DataFile;
        ef->data.resize(pack_buffer.size() * 4, 0);

        Longword* dest = (Longword*) (void*) &ef->data[0];
        for (int i = 0 ; i < pack_buffer.size() ; i++) {
            dest[i] = pack_buffer[i];
        }

        return ef;
*/
}

static PackParams create_pack_params(const shrinkler_parameters& parameters)
{
    return
    {
        .iterations = parameters.iterations,
        .length_margin = parameters.length_margin,
        .skip_length = parameters.skip_length,
        .match_patience = parameters.effort,
        .max_same_length = parameters.same_length
    };
}

std::vector<unsigned char> shrinkler::compress(const std::vector<unsigned char>& /*input*/)
{
    m_console.out << "Compressing..." << std::endl;
    RefEdgeFactory edge_factory(m_parameters.references);

    // TODO: unhardcode the progress thing (do we have that as an option, anyway?)
    auto pack_params = create_pack_params(m_parameters);
    crunch(&pack_params, &edge_factory, true);

    // TODO: actually return something
    return std::vector<unsigned char>();
}

}

// TODO: port stuff below
/*
* From Shrinkler.cpp:
*
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
