#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Song
{
private:
   std::string m_name;

   unsigned m_numChannels;
   unsigned m_numInstruments;

   struct Sample 
   {
       std::string name          = ""; // Sample name.
       uint16_t    length        = 0;  // Sample length in bytes.
       uint8_t     fileTune      = 0;  // Sample filetune. Only the lower nibble is valid.
       uint8_t     volume        = 0;  // Linear volume.
       uint16_t    repeatOffset  = 0;  // Sample repeat offset. Multiply by 2 to get position in bytes.
       uint16_t    repeatLength  = 0;  // Sample repeat length. Multiply by 2 to get length in bytes.

       Sample() {};
       Sample(const std::vector<char>& sampleBlock);
   };

   Sample *m_samples;

public:
    // Constructor.
    Song(std::vector<uint8_t>& songData);

    /* Getter methods. */
    const std::string& name() const
    {
        return m_name;
    }

};
