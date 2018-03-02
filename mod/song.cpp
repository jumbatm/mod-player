#include <iterator> // distance().
#include <exception> // runtime_error
#include <algorithm> // max_element().
#include <array>

#include <iostream>

#include "mod/song.hpp"
#include "mod/notemixer.hpp"

#include "sound/mixer.hpp"
#include "sound/sound.hpp"

#define SOFT_ASSERT(x) _soft_assert(__FILE__, __LINE__, x)
void _soft_assert(const char* filename, const unsigned line, bool condition)
{
    if (!condition)
    {
        std::cerr << "Soft assertion failed at " << filename << ":" << line << "\n";
    }
}

///////////////////////////////////////////////////////////////////////////////
// File reading.
///////////////////////////////////////////////////////////////////////////////

const static std::string fourThirtyOne(" M.K. M!K! FLT4 4CHN");
const static std::string sixThirtyOne(" 6CHN");
const static std::string eightThirtyOne(" 8CHN OCTA");

Song::Song(const std::vector<uint8_t>& songData)
{
    // Use an iterator so that we don't have to continually keep track of where we are
    // in the song data.
    std::vector<uint8_t>::const_iterator cit = songData.begin();

    // Read the title of the song.
    m_name = std::string(cit, cit + 20);
    
    cit += 20; // Can't put this compound assignment in function call, as functions calls are evaluated right to left.
    
    {   // Skip ahead and see how many instruments we have.
        // There can either be 15 or 31 instruments, so that's either at 1080 or 600 respectively.
        // TODO: Also check for the 15 instrument case.
        std::vector<uint8_t>::const_iterator ft = songData.begin() + 1080;
        std::string formatTag(ft, ft + 4);

        if (fourThirtyOne.find(formatTag))
        {
            m_numChannels = 4;
            m_numInstruments = 31;
        }
        else if (sixThirtyOne.find(formatTag))
        {
            m_numChannels = 6;
            m_numInstruments = 31;
        }
        else if (eightThirtyOne.find(formatTag))
        {
            m_numChannels = 8;
            m_numInstruments = 31;
        }
        else
        {
            // TODO: Throw some sort of exception.
            m_numChannels = 0;
            m_numInstruments = 0;
        }
    }

    // Now, we begin storing information about our instruments.
    // Every instrument's information is contained in 30 bytes.

    // Allocate memory for m_numInstruments instruments.
    m_samples = std::vector<Sample>(m_numInstruments);

    for (auto& sample : m_samples)
    {
        sample = Sample(std::vector<char>(cit, cit + 30));
        cit += 30;
    }

    // Read number of patterns in song to be played. (1 byte).
    m_numPatternsPlayed = *cit;

    cit += 1;

    // Read song end jump position (1 byte). If this is less than 127, cit
    // specifies where in the file to jump to at the song's end.
    m_songEndJumpPosition = *cit;

    cit += 1;

    // Read pattern table. (128 bytes). We only consider the first m_numPatternsPlayed bytes.
    m_patternTable = std::vector<uint8_t>(cit, cit + m_numPatternsPlayed);

    // The largest value in the pattern table is equal to the number of patterns - 1.
    // This is the most reliable way to determine this.
    m_numPatterns = (*(std::max_element(cit, cit + 128))) + 1;

    cit += 128;

    // Skip file format tag - we've already had a peek at it.
    cit += 4; 

    // Pattern and sample data. Usually less than 64, but can be up to 128.
    size_t patternDataLength = (m_numChannels * /* Bytes per note:*/ 4 * sizeof(uint8_t) * /* Number of lines per pattern:*/ 64 * m_numPatterns);
    m_patternData = std::vector<uint8_t>(cit, cit + patternDataLength);

    // Move `cit` past all the pattern data.
    cit += patternDataLength;

    // Assign every sample its voice. Samples can modify their own copy of their
    // voice.
    for (auto& sample : m_samples)
    {
        auto offset = std::distance<decltype(cit)>(songData.begin(), cit);
        sample.sampleData = std::vector<uint8_t>(songData.begin() + offset, songData.begin() + offset + sample.length);
        cit += sample.length; // Take advantage of information we already have.
    }

    SOFT_ASSERT(cit == songData.end());
}

////////////////////////////////////////////////////////////////////////////////
// Song playing.
////////////////////////////////////////////////////////////////////////////////
namespace {
class Playing
{
    const NoteMixer m_mixedSample;
    size_t index = 0;

    public:
    Playing(Sample& s, double scalefactor = 1.0) :
        m_mixedSample(s, scalefactor) {}
                

    uint8_t getSample()
    {
        std::cout << "Request for: " << index << "(" << size() << ")\n";
        return m_mixedSample.at(index++);
    }

    size_t size()
    {
        return m_mixedSample.size();
    }
};
} // end anonymous namespace

void Song::play()
{
    // Set up a small buffer and a currentlyPlaying vector.
    //
    // Read some amount from the pattern table. Add these to the
    // currentlyPlaying vector.
    //
    // Using the Notes' getAt(int) method called repeatedly, fill the buffer
    // with the correct samples, deducting from samplesLeft. 
    //
    // If samplesLeft is
    // 0, remove from vector. (Possibly use an implementation where the sample
    // isn't actually removed - we don't want to incur a vector resize, it's
    // suitable to just write over that vector the next time we want to insert
    // to list. The issue then is that we always have to iterate over every
    // value, unless we also keep track of how many we expect.
    //
    // Send the buffer to PulseAudio to be played. Blocking.
    //
    // Clear the buffer.
    //
    // Go to second step.

    // Testing:
    Sound::init(8192);

    Playing p(m_samples[4], 4);

    std::vector<uint8_t> buffer(p.size());

    for (;;)
    {
        for (auto& bufsam : buffer)
        {
            bufsam = p.getSample();
        }
        
        Sound::playRaw(&buffer[0], buffer.size());
    }

}

