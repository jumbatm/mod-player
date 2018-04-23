#pragma once
#include <cstdint>

#include "effect.hpp"

constexpr bool USE_PAL_VALUE = true;

class Note
{
    private:
        uint8_t m_sampleNumber = 0;
        uint16_t m_period = 0;
        effect_t m_effect = SILENCE;
        uint8_t m_argument = 0;
    public:
        Note(uint32_t d)
        {
            uint8_t *data = reinterpret_cast<uint8_t *>(&d);
            m_sampleNumber = (data[0] & (0xF << 4)) | ((data[2] & 0xF << 4) >> 4);
            m_period = ((data[0] & 0xF) << 8) | data[1];
            m_effect = static_cast<effect_t>(data[2] & 0xF);
            m_argument = data[3];
        }

        uint8_t index()
        {
            return m_sampleNumber;
        }

        double sampleRate()
        {
            double multiplier;

            if constexpr(USE_PAL_VALUE)
            {
                multiplier = 7093789.2;
            } 
            else 
            {
                multiplier = 7159090.2;
            }

            return multiplier / (2 * m_period);
        }

};
