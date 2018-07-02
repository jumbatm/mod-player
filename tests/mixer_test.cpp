#include "catch.hpp"
#include "sound/mixer.hpp"
#include <random>

namespace
{
    template <typename T>
    T getRandom()
    {
        return rand() % std::numeric_limits<T>::max();
    }
}

TEST_CASE("Static mix-in function works correctly")
{
    auto mixingFn = [](auto a, auto b)
    {
        return a + b - a*b / 256.0;
    };

    SECTION("Mixing a value with 0 with the function above leaves the value unaffected.")
    {
        REQUIRE(mixingFn(0, 255) == 255);
    }

    SECTION("Formula in class returns the same answers as the one above.")
    {
        // Perform this test 10 times.
        for (int i = 0; i < 10; ++i)
        {
            uint8_t a = getRandom<uint8_t>();
            uint8_t b = getRandom<uint8_t>();

            REQUIRE(Mixer<uint8_t>::mix(a, b) == mixingFn(a, b));
        }
    }

}

TEST_CASE("Mix-ins using the overloaded operator+ match the function above.")
{
    std::valarray<uint8_t> data;
    Mixer<uint8_t> m(std::move(data));
}
