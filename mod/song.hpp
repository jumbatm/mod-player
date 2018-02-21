#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <numeric>

#include <cstdio>
#include <iostream>

#include "mod/effects.hpp"

class Song
{
   private:
   std::string m_name;

   unsigned m_numChannels        = 0;
   unsigned m_numInstruments     = 0;

   // TODO: Make it so that the Sample and Note struct doesn't have to be
   // included in order to use the Song class.
   //
   // Possibly solved PIMPL?

   struct Sample 
   {
       std::string name          = ""; // Sample name.
       uint16_t    length        = 0;  // Sample length in bytes.
       uint8_t     fileTune      = 0;  // Sample filetune. 
       uint8_t     volume        = 0;  // Linear volume.
       uint16_t    repeatOffset  = 0;  // Sample repeat offset. 
       uint16_t    repeatLength  = 0;  // Sample repeat length.
      
       // Audio data of this sample.
       std::vector<uint8_t> sampleData;

       Sample() {};
       Sample(const std::vector<char>& sampleBlock);
   };

   struct Note
   {
       uint8_t index; // What sample is this?
       uint16_t period; // What period do we play the sample at?

       effect_t effect; // Effect applied to the sample.
       uint8_t argument; // Argument for this effect.

       // Ctor taking a raw word from a MOD file.
       Note(uint16_t word);
   };


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

    // TODO: Change to const std::vector<uint8_t>&, because we no longer make
    // modifications to songData - we now give each class its own copy of the
    // information that it might want to modify at will.
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
