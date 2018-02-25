#include "mod/notemixer.hpp"
#include "sound/mixer.hpp"

////////////////////////////////////////////////////////////////////////////////
// NoteMixer function definitions.
////////////////////////////////////////////////////////////////////////////////

uint8_t NoteMixer::at(size_t index) const
{
    return m_sample.sampleData.at(
            Mixer::helper::scale(m_startOffset + index, m_scaleFactor)
            );
}
