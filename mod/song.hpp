#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <numeric>
#include <cstdio>
#include <iostream>

#include "mod/sample.hpp"

class Song
{
   private:
   std::string m_name;

   unsigned m_numChannels        = 0;
   unsigned m_numInstruments     = 0;

    // Each sample object is stored here.
    std::vector<Sample> m_samples;

    // How many patterns get played in total?
    uint8_t m_numPatternsPlayed  = 0;    
   
    // What order do we play these patterns?
    std::vector<uint8_t> m_patternTable;

    // How many patterns are stored in the MOD file?
    uint8_t m_numPatterns        = 0; 

    // This is a copy of the pattern data.
    std::vector<uint32_t> m_patternData;

    uint8_t m_songEndJumpPosition;

public:
    // Constructor.
    Song(const std::vector<uint8_t>& songData);

    // Getter methods.
    const std::string& name() const
    {
        return m_name;
    }
    
    unsigned const& numInstruments() const
    {
        return m_numInstruments;
    }

    Sample& getInstrument(uint8_t index)
    {
        return m_samples.at(index);
    }

    // Manipulate sound data in some way.
    template<typename T, typename Callable>
    void for_each_sound_sample(Callable f)
    {
        for (Sample& s : m_samples)
        {
            for (T& sample : s.sampleData)
            {
                f(sample);
            }
        }
    }

    // Play the song through speakers.
    void play();

    // Print out all fields. Useful for debugging.
#ifdef DEBUG
    void print();
#endif
};
