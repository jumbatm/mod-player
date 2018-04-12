#pragma once

#include <cstdint>

namespace wordreader
{

    enum endian_t
    {
        BIG,
        LITTLE
    };

    enum class nibble_t : bool
    {
        LOW = 0,
        HIGH
    };


    template <typename T>
    T readword(uint8_t first, uint8_t second, endian_t type = BIG)
    {
        if (type == BIG)
        {
            // The most significant byte is at the lowest memory address.
            // ie, 0xHHLL, where H is the high byte and L is the low byte.
           return ((first << 8) | second);
        }
        else if (type == LITTLE)
        {

            // The least significant byte is in the highest memory address.
            // ie, 0xLLHH, where L is the low byte and H is the high byte.
            return ((second << 8) | (first));
        }
        else
        {
            return static_cast<T>(0);
        }

    }

    template <typename T = uint8_t>
    T readnibble(uint8_t byte, nibble_t which = nibble_t::LOW)
    {
        if (which == nibble_t::LOW)
        {
            return byte & 0xF;
        }
        else
        {
            return byte >> 8;
        }
    }
}
