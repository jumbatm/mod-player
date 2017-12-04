#pragma once

#include <cstdint>

namespace SoundPlayer
{
    void init(unsigned sampleRate);
    void playRaw(int8_t *samples, unsigned bufferSizeInBytes);
};
