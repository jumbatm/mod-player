#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Song
{
   public: // TODO: Make private again.
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
      
       // Where do I find the beginning of this sample's sound?
       std::vector<uint8_t>::const_iterator sampleData;

       Sample() {};
       Sample(const std::vector<char>& sampleBlock);
   };

    // Information for each sample is stored here.
    Sample *m_samples            = nullptr;

    // How many patterns get played in total?
    uint8_t m_numPatternsPlayed  = 0;    
   
    // What order do we play these patterns?
    std::vector<uint8_t> m_patternTable;

    // How many patterns are stored in the MOD file?
    uint8_t m_numPatterns        = 0; 

    // Where in the buffer do I find pattern data?
    // This stores where in the file to start looking. We can do this because it
    // is guaranteed that every pattern is 64 lines long.
    std::vector<uint8_t>::const_iterator m_patternData;

    uint8_t m_songEndJumpPosition;

public:
    // Constructor.
    Song(std::vector<uint8_t>& songData);
    // Destructor.
    ~Song();

    // Getter methods.
    const std::string& name() const
    {
        return m_name;
    }
    
    unsigned const& numInstruments() const
    {
        return m_numInstruments;
    }

    // Play the song through speakers.
    void play();
};
