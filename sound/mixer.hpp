#pragma once

#include <vector>
#include <limits>

namespace Mixer
{
    template <typename ElementType>
    ElementType mix(ElementType firstValue, ElementType secondValue)
    {
        // The "AB/256" from the formula above, for any type. Casted to avoid
        // integer division.
        double normaliser = firstValue * secondValue
            / static_cast<double>(std::numeric_limits<ElementType>::max() + 1.0);

        return firstValue + secondValue - normaliser;
    }

} // end namespace Mixer::util
