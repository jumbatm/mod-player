#include "sound/mixer.hpp"

#include "mod/song.hpp"

#include <stdexcept>
#include <cstdint>

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

    void mixIn(size_t offset, const uint8_t *const track, size_t trackSize, double scalefactor) // TODO: Overload for iterator.
    {
        // Check that the end of the added track doesn't go off the edge of the
        // current track.
        if (offset + trackSize > currentTrackSize)
        {
            throw std::runtime_error("Cannot fit specified track in currentTrack!");
        }

        // TODO: Use uint16_ts to get more resolution - something I'll have
        // to test.
        unsigned currentTrackIndex = 0;
        for (double trackIndex = 0; trackIndex < trackSize; trackIndex += scalefactor, ++currentTrackIndex)
        {
            // Using equation described from http://www.vttoth.com/CMS/index.php/technical-notes/68.
            // Instead of naively taking an average, we can do
            // Z = A + B - normalise(AB)
            
            auto& A = currentTrack[offset + currentTrackIndex];
            auto& B = track[static_cast<unsigned>(trackIndex)];

            A = A + B - (A*B)/256.0;

        }
    }

    TrackInfo getTrack()
    {
        if (currentTrack == nullptr) throw std::runtime_error("No current track - call newTrack first!");

        return { currentTrack, currentTrackSize };
    }


}
