#include "mod/notemixer.hpp"
#include "mod/effect.hpp"
#include "wordreader.hpp"

#include <iostream>

constexpr bool USE_PAL_VALUE = true;

namespace
{
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
                m_sampleNumber = (data[0] & (0xF << 4)) | ((data[2] & 0xF) >> 4);
                m_period = ((data[0] & 0xF) << 8) | data[1];
                m_effect = static_cast<effect_t>(data[2] & 0xF);
                m_argument = data[3];
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
}

NoteMixer NoteMixer::Create(Sample& samp, uint32_t d)
{
    Note n(d);
    return NoteMixer(samp, n.sampleRate());
}

////////////////////////////////////////////////////////////////////////////////
// NoteMixer function definitions.
////////////////////////////////////////////////////////////////////////////////

uint8_t NoteMixer::at(size_t index) const
{
    size_t scaledIndex = index * m_scaleFactor;

    if (scaledIndex < NoteMixer::size())
    {
        return m_sample.sampleData[scaledIndex];
    }
    else // We need to look at where to jump to because we've been asked for a value
        // outside of the sample's actual indices.
    {
        size_t idx = (scaledIndex % m_jumpPosition) + m_jumpPosition;

        return m_sample.sampleData[idx];
    }
}

uint8_t NoteMixer::next() 
{
    return NoteMixer::at(m_index++);
}
