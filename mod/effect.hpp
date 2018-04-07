#pragma once

enum effect_t
{
   // Just for us.
   SILENCE = -1,
   
   // These are in the same order they are defined, so
   // it's safe to static_cast directly to them.
   ARPEGGIO,
   SLIDE_UP,
   SLIDE_DOWN,
   TP, // = Tone Portamento.
   VIBRATO,
   TP_AND_VOLUME_SLIDE,
   TREMOLO,
   SET_PANNING_POSITION,
   SET_SAMPLE_OFFSET,
   VOLUME_SLIDE,
   POSITION_JUMP,
   SET_VOLUME,
   PATTERN_BREAK,
   EXTENDED_EFFECT,
   SET_SPEED,

   // And here is the list of extended effects. To static_cast directly
   // to them, first subtract 0xF.
   SET_FILTER,
   FINESLIDE_UP,
   FINESLIDE_DOWN,
   GLISSANDO_CONTROL,
   SET_VIBRATO_WAVEFORM,
   SET_FINETUNE,
   JUMP_TO_LOOP,
   SET_TREMOLO_WAVEFORM,
   _not_used_,
   RETRIG_NOTE,
   FINE_VOLUME_SLIDE_UP,
   FINE_VOLUME_SLIDE_DOWN,
   NOTE_CUT,
   NOTE_DELAY,
   PATTERN_DELAY,
   INVERT_LOOP
};
