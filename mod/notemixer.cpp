#include "mod/notemixer.hpp"
#include "sound/mixer.hpp"

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
        size_t idx = (m_jumpPosition + scaledIndex) % NoteMixer::size();

        return m_sample.sampleData[idx];
    }
}

uint8_t NoteMixer::next() 
{
    return NoteMixer::at(m_index++);
}
