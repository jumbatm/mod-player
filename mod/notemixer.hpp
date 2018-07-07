#pragma once
#include <cstdint>
#include <stdexcept>
#include <cmath>

#include "mod/song.hpp"
#include "mod/sample.hpp"

class NoteMixer
{
    const Sample& m_sample;     // The sample object we interpolate on.
    size_t m_startOffset = 0;   // Where in the sample data should we start?
    double m_scaleFactor = 1.0; // How much are we warping its play speed by?
    size_t m_jumpPosition = 0;  // Where to jump to when indexing to end of sample?

    size_t m_index = 0;

    public:
    NoteMixer(const Sample& s, double scaleFactor = 1.0, size_t startOffset = 0) : 
        m_sample(s),
        m_startOffset(startOffset),
        m_scaleFactor(scaleFactor),
        m_jumpPosition(m_sample.repeatOffset / (m_scaleFactor >= 1.0 ? m_scaleFactor : 1.0)),
        m_index(startOffset) 
    {
        if (NoteMixer::playRate == 0)
        {
            throw std::runtime_error("No player rate set for NoteMixer! Did you forget to set NoteMixer::playRate");
        }
    }
    
    // This class provides an interface as if the sample data was an infinite
    // array of values (with repeats of the jump position to the end). The below
    // functions enable this.

    // Return the sample at a certain index, overlapping to the jump position if
    // needed.
    uint8_t at(size_t index) const; // TODO: Consider templating this for the return type.

    // Get the sample at the internal index (m_index) and increment it.
    uint8_t next();

    // Query the size after scaling. This is The size can't get larger, so disallow
    // divisions less than 1.0.
    size_t size() const { return ::ceil(m_sample.sampleData.size() / (m_scaleFactor > 1.0 ? m_scaleFactor : 1.0)); }

    // Create NoteMixers based on the effect read.
    static NoteMixer Create(const Song& from, uint32_t data);
    static unsigned playRate;     // What's our audio player's sample rate?

};
