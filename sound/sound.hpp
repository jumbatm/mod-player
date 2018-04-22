#pragma once

#include <cstdint>

namespace Sound
{
    void init(unsigned sampleRate);
    void playRaw(uint8_t const *samples, unsigned bufferSizeInBytes);
    unsigned getSampleRate();
};
