#include "mod/sample.hpp"
#include "wordreader.hpp"

#include <exception>
#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////
// Sample class implementation
///////////////////////////////////////////////////////////////////////////////

Sample::Sample(const std::vector<char>& sampleBlock)
{
    // First, check that sampleBlock has a length of 30.
    if (sampleBlock.size() != 30)
    {
        throw std::runtime_error("Sample::Sample: Sample block not of correct size!");
    }

    // Use another iterator here to keep track of where we are in the sample block.
    std::vector<char>::const_iterator cit = sampleBlock.begin();

    // First 22 bytes is the sample's name.
    name = std::string(cit, cit + 22);
    
    cit += 22;

    // Next 2 bytes are the sample length in big-end words.
    // ie, most significant byte in the lowest address.
    length = 2 * wordreader::readword<decltype(length)>(*cit, *(cit + 1), wordreader::BIG);

    cit += 2;

    // Sample's finetune. Only the lower 4 bits are valid.
    fileTune = *cit & 0xF;
   
    ++cit;

    // Sample's linear volume. This is 1 byte in length.
    volume = *cit;

    ++cit;

    // Sample's repeat offset in 2-byte words.
    repeatOffset = 2 * wordreader::readword<decltype(repeatOffset)>(*cit, *(cit + 1), wordreader::BIG);

    cit += 2;

    // Sample's repeat length in 2-byte words.
    repeatLength = 2 * wordreader::readword<decltype(repeatLength)>(*cit, *(cit + 1), wordreader::BIG);

    cit += 2;

    // The final field is sampleData - this information must be provided
    // separately. It is done so in song's constructor.
}

