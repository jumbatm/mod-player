#pragma once

#include "mod/sample.hpp"

struct Note
{
   uint8_t index; // What sample is this?
   uint16_t period; // What period do we play the sample at?

   effect_t effect; // Effect applied to the sample.
   uint8_t argument; // Argument for this effect.

   // Ctor taking a raw word from a MOD file.
   Note(uint16_t word);

   // TODO: This is a intermediary representation. It should be able to access
   // the sample that the index refers to - actually, now that I think about it,
   // this class should be the representation of the actual data, and we should
   // use another class which connects the Song to the Sample.
};
