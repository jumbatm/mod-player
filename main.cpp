#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "mod/song.hpp"
#include "sound/sound.hpp"
#include "sound/mixer.hpp"

enum ExitCodes
{
    SUCCESS = 0,
    NOT_ENOUGH_ARGUMENTS,
    FILE_NOT_FOUND
};

void displayUsage()
{
    std::cout << "Usage: ./mplay <filename.mod>\n";
}

int main(int argc, char** argv)
{
    // Check we have the right amount of arguments.
    if (argc != 2)
    {
        displayUsage();
        return NOT_ENOUGH_ARGUMENTS;
    }

    // If we do, look for the file and write it into a buffer.
    std::ifstream file(argv[1], std::ios::binary);
    
    if (!file.is_open())
    { 
        std::cout << "File not found!" << std::endl; 
        return FILE_NOT_FOUND;
    }

    // Read the data into a byte vector.
    std::vector<uint8_t> songData(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

    songData.shrink_to_fit();
    
    // From now on, we just work from the buffer.
    file.close();

    // Construct a new song object from this data.
    Song song(songData);

    // Convert the song data to unsigned 8 bit, because PulseAudio doesn't do
    // signed 8 bit natively.
    song.for_each_sound_sample<uint8_t>([](uint8_t& byte) 
    {
        byte = reinterpret_cast<int8_t&>(byte) + 128;
    });

    song.play();

    return SUCCESS;
}
