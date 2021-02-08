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

#include "shrinkler.ipp"

#include <boost/numeric/conversion/cast.hpp>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include "fmt/core.h"
#include "console.hpp"
#include "shrinkler.hpp"

namespace libgbaic
{

using fmt::format;
using std::runtime_error;
using std::vector;

static void packData2(console& console, unsigned char* data, int data_length, int zero_padding, PackParams* params, Coder* result_coder, RefEdgeFactory* edge_factory, bool show_progress) {
    MatchFinder finder(data, data_length, 2, params->match_patience, params->max_same_length);
    LZParser parser(data, data_length, zero_padding, finder, params->length_margin, params->skip_length, edge_factory);
    result_size_t real_size = 0;
    result_size_t best_size = (result_size_t)1 << (32 + 3 + Coder::BIT_PRECISION);
    int best_result = 0;
    vector<LZParseResult> results(2);
    CountingCoder* counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
    LZProgress* progress;
    if (show_progress) {
        progress = new PackProgress();
    }
    else {
        progress = new NoProgress();
    }
    CONSOLE_OUT(console) << "Original: " << data_length << std::endl;
    for (int i = 0; i < params->iterations; i++) {
        // Parse data into LZ symbols
        LZParseResult& result = results[1 - best_result];
        Coder* measurer = new SizeMeasuringCoder(counting_coder);
        measurer->setNumberContexts(LZEncoder::NUMBER_CONTEXT_OFFSET, LZEncoder::NUM_NUMBER_CONTEXTS, data_length);
        finder.reset();
        result = parser.parse(LZEncoder(measurer), progress);
        delete measurer;

        // Encode result using adaptive range coding
        vector<unsigned> dummy_result;
        RangeCoder* range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS, dummy_result);
        real_size = result.encode(LZEncoder(range_coder));
        range_coder->finish();
        delete range_coder;

        // Choose if best
        if (real_size < best_size) {
            best_result = 1 - best_result;
            best_size = real_size;
        }

        // Print size
        CONSOLE_OUT(console) << format("Pass {}: {:.3f}", i + 1, real_size / (double)(8 << Coder::BIT_PRECISION)) << std::endl;

        // Count symbol frequencies
        CountingCoder* new_counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
        result.encode(LZEncoder(counting_coder));

        // New size measurer based on frequencies
        CountingCoder* old_counting_coder = counting_coder;
        counting_coder = new CountingCoder(old_counting_coder, new_counting_coder);
        delete old_counting_coder;
        delete new_counting_coder;
    }
    delete progress;
    delete counting_coder;

    results[best_result].encode(LZEncoder(result_coder));
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

vector<unsigned char> shrinkler::compress(const vector<unsigned char>& data)
{
    CONSOLE_OUT(m_console) << "Compressing..." << std::endl;

    RefEdgeFactory edge_factory(m_parameters.references);
    auto pack_params = create_pack_params(m_parameters);

    // For the time being we do not allow progress updates using ANSI escape sequences.
    // Problem is that in the past the Windows console did not support ANSI escape sequences at all.
    // On more recent versions of Windows it does, but this needs to be probed for and enabled:
    // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences.
    // Not worth the trouble for the time being.
    auto packed_bytes = crunch(data, pack_params, edge_factory, false);

    CONSOLE_VERBOSE(m_console) << format("References considered: {}", edge_factory.max_edge_count) << std::endl;
    CONSOLE_VERBOSE(m_console) << format("References discarded: {}", edge_factory.max_cleaned_edges) << std::endl;
    CONSOLE_VERBOSE(m_console) << format("Uncompressed data size: {} bytes", data.size()) << std::endl;
    CONSOLE_VERBOSE(m_console) << format("Final compressed data size: {} bytes", packed_bytes.size()) << std::endl;

    if (edge_factory.max_edge_count > m_parameters.references)
    {
        CONSOLE_OUT(m_console) << "Note: compression may benefit from a larger reference buffer (-r option)" << std::endl;
    }

    return packed_bytes;
}

vector<unsigned char> shrinkler::crunch(const vector<unsigned char>& data, PackParams& params, RefEdgeFactory& edge_factory, bool show_progress)
{
    // Shrinkler code uses non-const buffers all over the place. Let's create a copy then.
    vector<unsigned char> non_const_data = data;

    // Compress and verify
    vector<uint32_t> pack_buffer = compress(non_const_data, params, edge_factory, show_progress);
    int margin = verify(non_const_data, pack_buffer);
    CONSOLE_VERBOSE(m_console) << "Minimum safety margin for overlapped decrunching: " << margin << std::endl;

    // Convert to array of bytes
    vector<unsigned char> packed_bytes;
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

int shrinkler::verify(vector<unsigned char>& data, vector<uint32_t>& pack_buffer)
{
    CONSOLE_VERBOSE(m_console) << "Verifying..." << std::endl;

    RangeDecoder decoder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);
    LZDecoder lzd(&decoder);

    // Verify data
    LZVerifier verifier(0, &data[0], boost::numeric_cast<int>(data.size()), boost::numeric_cast<int>(data.size()));
    decoder.reset();
    decoder.setListener(&verifier);
    if (!lzd.decode(verifier))
    {
        throw runtime_error("INTERNAL ERROR: could not verify decompressed data");
    }

    // Check length
    auto decompressed_data_size = verifier.size();
    if (static_cast<std::make_unsigned<decltype(decompressed_data_size)>::type>(decompressed_data_size) != data.size())
    {
        throw runtime_error(format("INTERNAL ERROR: decompressed data has incorrect length ({}, should have been {})", verifier.size(), data.size()));
    }

    return verifier.front_overlap_margin + pack_buffer.size() * 4 - data.size();
}

vector<uint32_t> shrinkler::compress(vector<unsigned char>& data, PackParams& params, RefEdgeFactory& edge_factory, bool show_progress)
{
    vector<uint32_t> pack_buffer;
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);

    // Crunch the data
    range_coder.reset();
    packData2(m_console, &data[0], boost::numeric_cast<int>(data.size()), 0, &params, &range_coder, &edge_factory, show_progress);
    range_coder.finish();

    return pack_buffer;
}

}
