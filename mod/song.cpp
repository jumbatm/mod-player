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
    size_t patternDataLength = (m_numChannels * /* 4 Bytes per note:*/ sizeof(uint32_t) * /* Number of lines per pattern:*/ 64 * m_numPatterns);
    m_patternData = std::vector<uint32_t>(cit, cit + patternDataLength);

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

    SOFT_ASSERT((std::cout << std::distance(cit, songData.cend()), cit == songData.end()));
}

////////////////////////////////////////////////////////////////////////////////
// Song playing.
////////////////////////////////////////////////////////////////////////////////


constexpr unsigned SAMPLE_RATE = 8192;

void Song::play()
{
    Sound::init(SAMPLE_RATE);
    NoteMixer::playRate = SAMPLE_RATE;

    NoteMixer n(m_samples[4], SAMPLE_RATE);

    std::vector<uint8_t> buffer(10);

    for (;;)
    {
        for (auto& bufsam : buffer)
        {
            bufsam = n.next();
        }
        
        Sound::playRaw(&buffer[0], buffer.size());
    }

}

////////////////////////////////////////////////////////////////////////////////
// Print function for debugging.
////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void Song::print()
{
#define NAME_AND_VALUE(VAR) #VAR << ": " << VAR << "[END]\n"
    std::cout 
        << NAME_AND_VALUE(m_name) 
        << NAME_AND_VALUE(m_numChannels) 
        << NAME_AND_VALUE(m_numInstruments) 
        << NAME_AND_VALUE((int)m_numPatternsPlayed)
        << NAME_AND_VALUE((int)m_numPatterns) 
        << NAME_AND_VALUE((int)m_songEndJumpPosition);

    for (Sample& samp : m_samples)
    {
        std::cout << samp.name << "[END]\n";
    }
#undef NAME_AND_VALUE
}
#endif // ifdef DEBUG
