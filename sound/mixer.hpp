#pragma once

#include <cstdint>
#include <cstddef>

namespace Mixer
{
    struct TrackInfo
    {
        uint8_t const* data;
        size_t const size;
    };

    namespace helper
    {
        size_t scale(size_t index, double scalefactor);
    }

    void newTrack(unsigned sampleSize); // Initialise a new track.
    void mixIn(size_t offset, const uint8_t *const track, size_t size, double scalefactor = 1.0); // Mix in new track.
    TrackInfo getTrack();
}
