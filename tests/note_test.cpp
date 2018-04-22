#include "catch.hpp"

#include "mod/note.hpp"

#include <endian.h>

TEST_CASE("Note extracts bits correctly")
{
    //       wX   xx   yZ   zz
    //     0x12 0x34 0x56 0x78
    //
    // Sample number:  wy
    // Sample period: Xxx
    // Effect       :   Z
    // Argument     :  zz 

    uint32_t num{(BYTE_ORDER == BIG_ENDIAN) ? 0x12345678 : 0x78563412};
    // So that the first byte is always 0x12, next byte is always 0x34, etc.

    Note n(num);

    REQUIRE(n.m_sampleNumber == 0x15);
    REQUIRE(n.m_period == 0x234);
    REQUIRE(n.m_effect == 0x6);
    REQUIRE(n.m_argument == 0x78);
}
