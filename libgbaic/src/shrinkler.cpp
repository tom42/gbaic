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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4018 4100 4244 4389 4456 4458)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "HunkFile.h"
#include "Pack.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#undef _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "fmt/core.h"
#include "console.hpp"
#include "shrinkler.hpp"

namespace libgbaic
{

using fmt::format;

// TODO: do we need pointers all over the place here?
static std::vector<uint32_t> compress(std::vector<unsigned char>& data, PackParams* params, RefEdgeFactory* edge_factory, bool show_progress)
{
    vector<uint32_t> pack_buffer;
    RangeCoder* range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer); // TODO: does this need to be a pointer?

    // Print compression status header
    // TODO: do we want this? Like that, with printfs?
    const char* ordinals[] = { "st", "nd", "rd", "th" };
    printf("Original");
    for (int p = 1; p <= params->iterations; p++) {
        printf("  After %d%s pass", p, ordinals[min(p, 4) - 1]);
    }
    printf("\n");

    // Crunch the data
    // TODO: remove printfs?
    range_coder->reset();
    packData(&data[0], data.size(), 0, params, range_coder, edge_factory, show_progress);
    range_coder->finish();
    printf("\n\n");
    fflush(stdout);

    return pack_buffer;
}

// TODO: verify all of this!
// TODO: are we sure we don't want to pass data by refernece?
int verify(std::vector<unsigned char> data, vector<uint32_t>& pack_buffer) {
    printf("Verifying... ");
    fflush(stdout);
    RangeDecoder decoder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);
    LZDecoder lzd(&decoder);

    // Verify data
    bool error = false;
    LZVerifier verifier(0, &data[0], data.size(), data.size());
    decoder.reset();
    decoder.setListener(&verifier);
    if (!lzd.decode(verifier)) {
        error = true;
    }

    // Check length
    // TODO: cast to size_t silences about != signed/unsigned mismatch. Is this REALLY a problem?
    if (!error && (size_t)verifier.size() != data.size()) {
        printf("Verify error: data has incorrect length (%d, should have been %d)!\n", verifier.size(), (int)data.size());
        error = true;
    }

    if (error) {
        // TODO: need to remove this: this mentions Blueberry, who maybe does not want bugreports from gbaic users.
        internal_error();
    }

    printf("OK\n\n");

    return verifier.front_overlap_margin + pack_buffer.size() * 4 - data.size();
}

// TODO: do we need pointers all over the place here (already on the args)
// TODO: get rid of printf all over the place
static std::vector<unsigned char> crunch(const std::vector<unsigned char>& data, PackParams* params, RefEdgeFactory* edge_factory, bool show_progress)
{
    // Shrinkler code uses non-const buffers all over the place. Let's create a copy then.
    std::vector<unsigned char> non_const_data = data;

    // Compress and verify
    vector<uint32_t> pack_buffer = compress(non_const_data, params, edge_factory, show_progress);
    int margin = verify(data, pack_buffer);
    printf("Minimum safety margin for overlapped decrunching: %d\n\n", margin);

    // Convert to array of bytes
    std::vector<unsigned char> packed_bytes;
    packed_bytes.reserve(pack_buffer.size() * sizeof(pack_buffer[0]));
    for (auto word : pack_buffer)
    {
        packed_bytes.push_back(word & 0xff);
        packed_bytes.push_back((word >> 8) & 0xff);
        packed_bytes.push_back((word >> 16) & 0xff);
        packed_bytes.push_back((word >> 24) & 0xff);
    }

    return packed_bytes;
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

std::vector<unsigned char> shrinkler::compress(const std::vector<unsigned char>& data)
{
    m_console.out << "Compressing..." << std::endl;
    RefEdgeFactory edge_factory(m_parameters.references);

    // TODO: unhardcode the progress thing (do we have that as an option, anyway?)
    auto pack_params = create_pack_params(m_parameters);
    auto packed_bytes = crunch(data, &pack_params, &edge_factory, true);

    // TODO: at best that's verbose output, no?
    m_console.out << fmt::format("References considered: {}", edge_factory.max_edge_count) << std::endl;
    m_console.out << fmt::format("References discarded: {}", edge_factory.max_cleaned_edges) << std::endl;

    return packed_bytes;
}

}

// TODO: port stuff below
/*
* From Shrinkler.cpp:
*
        printf("Saving file %s...\n\n", outfile);
        crunched->save(outfile);

        printf("Final file size: %d\n\n", crunched->size());
        delete crunched;

        if (edge_factory.max_edge_count > references.value) {
            printf("Note: compression may benefit from a larger reference buffer (-r option).\n\n");
        }
*/
