#pragma once

#include <cstdint>
#include <cstddef>
#include <valarray>
#include <limits>

template <typename ElementType>
class Mixer
{
    // So it turns out we can't just naively take an average, because if the
    // other track is full of zeroes, we end up hearing the first track, but at
    // half amplitude. Instead, we use equation described from
    // http://www.vttoth.com/CMS/index.php/technical-notes/68. 
    // Z = A + B - (AB/256).
    //
    // This class simply wraps a valarray and makes it perform this operation on
    // its addition and subtraction operators, . For ease of use, it also provides
    // iterator support.
 
    public:
    // Default construct.
    Mixer() : data()
    {
    }

    ElementType& operator[](size_t index)
    {
        return data[index];
    }

    Mixer<ElementType>& operator+(const std::valarray<ElementType>& other)
    {
        return Mixer{data + other - (data * other) 
            / static_cast<double>(std::numeric_limits<ElementType>::max())};
        // See comment below.
    }

    // A helper function to apply the formula described above. Automatically
    // performs scaling to the maximum value ResultType can hold.
    template <typename Type, typename ResultType = Type>
    static ResultType mix(Type firstValue, Type secondValue)
    {
        // The "AB/256" from the formula above, for any type. Casted to avoid
        // integer division.
        double normaliser = firstValue * secondValue
            / static_cast<double>(std::numeric_limits<ResultType>::max());

        return firstValue + secondValue - normaliser;
    }


    private:
    std::valarray<ElementType> data;
};
