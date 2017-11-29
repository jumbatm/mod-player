#include <numeric>
#include <iterator>
#include <iostream>
#include <exception>

#include "include/wordreader.hpp"
#include "include/song.hpp"

/** Song class implementation **/

const static std::string fourThirtyOne(" M.K. M!K! FLT4 4CHN");
const static std::string sixThirtyOne(" 6CHN");
const static std::string eightThirtyOne(" 8CHN OCTA");

Song::Song(std::vector<uint8_t>& songData)
{
    // Use an iterator so that we don't have to continually keep track of where we are
    // in the song data.
    std::vector<uint8_t>::iterator it = songData.begin();

    // Read the title of the song.
    m_name = std::string(it, it + 20);
    it += 20; // Can't put this compound assignment in function call, as functions calls are evaluated right to left.
    
    {  // Skip ahead and see how many instruments we have. This at position 1080d.
        std::vector<uint8_t>::iterator ft = songData.begin() + 1080;
        std::string formatTag(ft, ft + 4);

        std::cout << formatTag << std::endl;

        if (fourThirtyOne.find(formatTag))
        {
            std::cout << "Found match!" << std::endl;
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
    

}

/** Sample class implementation. **/
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
    this->name = std::string(it, it + 22);
    
    it += 22;

    // Next 2 bytes are the sample length in big-end words.
    // ie, most significant byte in the lowest address.
    length = 2 * wordreader::readword<decltype(length)>(*it, *(it + 1), wordreader::BIG);

    it += 2;

    // Sample's filetune. Only the lower 4 bits are valid.
    fileTune = *it & 0xF;
   
    ++it;

    // Sample's linear volume. This is 1 byte in length.
    volume = *it;

    ++it;

    // Sample's repeat length in 2-byte words.
}

