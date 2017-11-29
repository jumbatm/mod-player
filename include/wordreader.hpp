#pragma once

#include <cstdint>

namespace wordreader
{

    enum endian_t
    {
        BIG,
        LITTLE
    };


    template <typename T>
    T readword(int8_t first, int8_t second, endian_t type  = BIG_ENDIAN)
    {
        if (type == BIG_ENDIAN)
        {
            // The most significant byte is at the lowest memory address.
            // ie, 0xHHLL, where H is the high byte and L is the low byte.
           return ((first << 8) | second);
        }
        else if (type == LITTLE_ENDIAN)
        {

            // The least significant byte is in the highest memory address.
            // ie, 0xLLHH, where L is the low byte and H is the high byte.
            return ((second << 8) | (first));
        }
        else
        {
            return (T)0;
        }

    }
}
