#include "sound/mixer.hpp"
#include "catch.hpp"

TEST_CASE("Mixing function works correctly")
{
    SECTION(
        "Mixing a value with 0 with the function above leaves the value "
        "unaffected.")
    {
        REQUIRE(Mixer::mix(0, 255) == 255);
        REQUIRE(Mixer::mix(255, 0) == 255);
    }
}
