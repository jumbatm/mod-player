#pragma once
#include <cstdint>

#include "mod/sample.hpp"

class NoteMixer
{
    const Sample& m_sample; // The sample object we interpolate on.
    size_t m_startOffset = 0; // Where in the sample data should we start?
    double m_scaleFactor = 1.0; // How much are we warping its play speed by?
    size_t m_jumpPosition = 0; // Where to jump to when indexing to end of sample?
    
    size_t m_index = 0;

    public:
    NoteMixer(Sample& s, double scaleFactor = 1.0, size_t startOffset = 0) : 
        m_sample(s),
        m_startOffset(startOffset),
        m_scaleFactor(scaleFactor),
        m_jumpPosition(m_sample.repeatOffset / m_scaleFactor),
        m_index(startOffset) {}

    // Return the sample at a certain index.
    uint8_t at(size_t index) const; // TODO: Consider templating this for the return type.

    // Get the sample at the internal index and increment.
    uint8_t next();

    // Query the size after scaling.
    size_t size() const { return m_sample.sampleData.size() / (m_scaleFactor > 1.0 ? m_scaleFactor : 1.0); }

    static NoteMixer Create(Sample& samp, uint32_t data);

};

