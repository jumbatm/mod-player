#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Song
{
private:
   std::string m_name;

   unsigned m_numChannels        = 0;
   unsigned m_numInstruments     = 0;

   struct Sample 
   {
       std::string name          = ""; // Sample name.
       uint16_t    length        = 0;  // Sample length in bytes.
       uint8_t     fileTune      = 0;  // Sample filetune. 
       uint8_t     volume        = 0;  // Linear volume.
       uint16_t    repeatOffset  = 0;  // Sample repeat offset. 
       uint16_t    repeatLength  = 0;  // Sample repeat length.

       Sample() {};
       Sample(const std::vector<char>& sampleBlock);
   };

   Sample *m_samples              = nullptr;
   
   uint8_t m_numPatternsPlayed    = 0;
   std::vector<uint8_t> m_patternTable; 

   uint8_t m_numPatterns          = 0;

   uint8_t m_songEndJumpPosition;
public:
    // Constructor.
    Song(std::vector<uint8_t>& songData);
    // Destructor.
    ~Song();

    /* Getter methods. */
    const std::string& name() const
    {
        return m_name;
    }

};
