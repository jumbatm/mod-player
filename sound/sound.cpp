#include "sound/sound.hpp"

#include <pulse/error.h>
#include <pulse/simple.h>

#include <exception>
#include <iostream>  // TODO: (See line 39).

namespace Sound
{
namespace detail
{
pa_sample_spec sampleSpec;
pa_simple *soundServer;
unsigned sampleRate;
}

void init(unsigned sampleRate_)
{
    detail::sampleRate = sampleRate_;

    // Connect to the PulseAudio sound server.
    detail::sampleSpec.format = PA_SAMPLE_U8;  // Use unsigned 8 bit PCM
    detail::sampleSpec.rate = sampleRate_;
    detail::sampleSpec.channels = 1;

    // Must remember to static_cast<unsigned char> before feeding to PulseAudio.
    // This is because the sample data in MOD files is in two's complement
    // signed 8 bit.

    int error;
    detail::soundServer =
        pa_simple_new(NULL,                 // Server name.
                      "Mod Player",         // Client name.
                      PA_STREAM_PLAYBACK,   // Recording or playback?
                      NULL,                 // Sink name.
                      "Mod Player",         // Stream name.
                      &detail::sampleSpec,  // Sample spec to use.
                      NULL,                 // Channel map.
                      NULL,                 // Buffering attributes.
                      &error                // Error out variable.
                      );

    if (!detail::soundServer)
    {
        std::cout << pa_strerror(error)
                  << std::endl;  // TODO: Format this into the exception string.
        throw std::runtime_error("Failed to make soundServer.");
    }
}

void playRaw(uint8_t const *rawData, unsigned size)
{
    int error;

    pa_simple_write(detail::soundServer, rawData, size, &error);

    if (error < 0)
    {
        throw std::runtime_error("Failed to play audio!");
    }
}

unsigned getSampleRate() { return detail::sampleRate; }
}
