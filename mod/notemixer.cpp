#include "mod/notemixer.hpp"
#include "sound/mixer.hpp"

////////////////////////////////////////////////////////////////////////////////
// NoteMixer function definitions.
////////////////////////////////////////////////////////////////////////////////

uint8_t NoteMixer::at(size_t index) const
{
    if (index < size())
    {
        // TODO: Test and confirm that we can never go wrong using unsafe
        // indexing[]
        return m_sample.sampleData.at(index);
    }
    else // Attempt to access a value outside of range.
    {
        return m_sample.sampleData.at(
                m_jumpPosition + (index % size())
                );
    }
}
