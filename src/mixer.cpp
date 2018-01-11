#include "sound/mixer.hpp"

#include <stdexcept>

namespace Mixer
{
    namespace
    {
        uint8_t *currentTrack = nullptr;
        size_t currentTrackSize = 0;
    }

    void newTrack(unsigned sampleSize)
    {
        // Check if there's currently an allocated track.
        if (currentTrack != nullptr)
        {
            delete[] currentTrack;

            currentTrack = nullptr;
            currentTrackSize = 0;
        }

        currentTrackSize = sampleSize;
        currentTrack = new uint8_t[sampleSize]();

    }

    void mixIn(size_t offset, uint8_t const* track, size_t trackSize) // TODO: Overload for iterator.
    {
        // Check that the end of the added track doesn't go off the edge of the
        // current track.
        if (offset + trackSize > currentTrackSize)
        {
            throw std::runtime_error("Cannot fit specified track in currentTrack!");
        }

        // Mix the two tracks by performing an average + addition.
        // CONSIDER: Use uint16_ts to get more resolution - something I'll have
        // to test.
        for (size_t i = 0; i < trackSize; ++i)
        {
            currentTrack[offset + i] >>= 1; // Divide by 2.
            currentTrack[offset + i] += track[i] >> 1;
        }
    }

    TrackInfo getTrack()
    {
        if (currentTrack == nullptr) throw std::runtime_error("No current track - call newTrack first!");

        return { currentTrack, currentTrackSize };
    }


}
