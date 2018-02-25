#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <numeric>

#include <cstdio>
#include <iostream>

#include "mod/effects.hpp"
#include "mod/sample.hpp"
#include "mod/note.hpp"

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

    // Where in the buffer do I find pattern data?
    // This stores where in the file to start looking. We can do this because it
    // is guaranteed that every pattern is 64 lines long.
    std::vector<uint8_t> m_patternData;

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

    // Manipulate sound data in some way.
    template<typename T>
    void for_each_sound_sample(std::function<void(T&)> f)
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
};
