#pragma once

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
    // A helper function to apply the formula described above. Automatically
    // performs scaling to the maximum value ResultType can hold.
    static ElementType mix(ElementType firstValue, ElementType secondValue)
    {
        // The "AB/256" from the formula above, for any type. Casted to avoid
        // integer division.
        double normaliser = firstValue * secondValue
            / static_cast<double>(std::numeric_limits<ElementType>::max() + 1.0);

        return firstValue + secondValue - normaliser;
    }

    // Default construct.
    Mixer() : data()
    {
    }

    // Construct from a valarray.
    Mixer(std::valarray<ElementType>&& toOwn) 
        : data(std::move(toOwn))
    {
    }

    ElementType& operator[](size_t index)
    {
        return data[index];
    }

    Mixer<ElementType>& operator+(const std::valarray<ElementType>& other)
    {
        // Unfortunately, valarray doesn't seem to provide a way to modify
        // elements in-place. They have an `apply` method, but this, for all
        // intents and purposes, generates a new valarray. Therefore, one of the
        // tests for this class is to verify that this operator produces results
        // identical to the one above.
        return Mixer{data + other - (data * other) 
            / static_cast<double>(std::numeric_limits<ElementType>::max())};
    }

    private:
    std::valarray<ElementType> data;
};
