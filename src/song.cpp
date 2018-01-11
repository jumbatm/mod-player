#include <iterator> // FUNC: std::distance().
#include <exception> // FUNC: std::runtime_error().
#include <utility> // FUNC:
#include <algorithm> // FUNC: std::max_element().

#include "wordreader.hpp"
#include "mod/song.hpp"

///////////////////////////////////////////////////////////////////////////////
// Song class implementation
///////////////////////////////////////////////////////////////////////////////

const static std::string fourThirtyOne(" M.K. M!K! FLT4 4CHN");
const static std::string sixThirtyOne(" 6CHN");
const static std::string eightThirtyOne(" 8CHN OCTA");

Song::Song(std::vector<uint8_t>& songData)
{
    // Use an iterator so that we don't have to continually keep track of where we are
    // in the song data.
    std::vector<uint8_t>::const_iterator it = songData.begin();

    // Read the title of the song.
    m_name = std::string(it, it + 20);
    it += 20; // Can't put this compound assignment in function call, as functions calls are evaluated right to left.
    
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
    m_samples = new Sample[m_numInstruments]();

    for (unsigned i = 0; i < m_numInstruments; ++i)
    {
        m_samples[i] = Sample(std::vector<char>(it, it + 30));
        it += 30;
    }

    // Read number of patterns in song to be played. (1 byte).
    m_numPatternsPlayed = *it;

    it += 1;

    // Read song end jump position (1 byte). If this is less than 127, it
    // specifies where in the file to jump to at the song's end.
    m_songEndJumpPosition = *it;

    it += 1;

    // Read pattern table. (128 bytes). We only consider the first m_numPatternsPlayed bytes.
    m_patternTable = std::move(std::vector<uint8_t>(it, it + m_numPatternsPlayed));

    // The largest value in the pattern table is equal to the number of patterns - 1.
    // This is the most reliable way to determine this.
    m_numPatterns = (*(std::max_element(it, it + 128))) + 1;

    it += 128;

    // Skip file format tag - we've already had a peek at it.
    it += 4;

    // Pattern and sample data. Usually less than 64, but can be up to 128.
    m_patternData = it;

    // Move `it` past all the pattern data.
    it += (m_numChannels * /* Bytes per note:*/ 4 * sizeof(uint8_t) * /* Number of lines per pattern:*/ 64 * m_numPatterns);

    // We should now be at the sample data. Assign every sample its voice.
    for (unsigned i = 0; i < m_numInstruments; ++i)
    {
        m_samples[i].sampleData = it;
        it += m_samples[i].length; // Take advantage of information we already have.
    }

}

Song::~Song()
{
    delete[] m_samples;
}

///////////////////////////////////////////////////////////////////////////////
// Sample class implementation
///////////////////////////////////////////////////////////////////////////////

Song::Sample::Sample(const std::vector<char>& sampleBlock)
{
    // First, check that sampleBlock has a length of 30.
    if (sampleBlock.size() != 30)
    {
        throw std::runtime_error("Sample::Sample: Sample block not of correct size!");
    }

    // Use another iterator here to keep track of where we are in the sample block.
    std::vector<char>::const_iterator it = sampleBlock.begin();

    // First 22 bytes is the sample's name.
    name = std::string(it, it + 22);
    
    it += 22;

    // Next 2 bytes are the sample length in big-end words.
    // ie, most significant byte in the lowest address.
    length = 2 * wordreader::readword<decltype(length)>(*it, *(it + 1), wordreader::BIG);

    it += 2;

    // Sample's finetune. Only the lower 4 bits are valid.
    fileTune = *it & 0xF;
   
    ++it;

    // Sample's linear volume. This is 1 byte in length.
    volume = *it;

    ++it;

    // Sample's repeat offset in 2-byte words.
    repeatOffset = 2 * wordreader::readword<decltype(repeatOffset)>(*it, *(it + 1), wordreader::BIG);

    it += 2;

    // Sample's repeat length in 2-byte words.
    repeatLength = 2 * wordreader::readword<decltype(repeatLength)>(*it, *(it + 1), wordreader::BIG);

    it += 2;
}

