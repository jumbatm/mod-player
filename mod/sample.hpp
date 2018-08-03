#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct Sample
{
    std::string name = "";      // Sample name.
    uint16_t length = 0;        // Sample length in bytes.
    uint8_t fileTune = 0;       // Sample filetune.
    uint8_t volume = 0;         // Linear volume.
    uint16_t repeatOffset = 0;  // Sample repeat offset.
    uint16_t repeatLength = 0;  // Sample repeat length.

    // Audio data of this sample.
    std::vector<uint8_t> sampleData;

    Sample(){};
    Sample(const std::vector<char> &sampleBlock);
};
