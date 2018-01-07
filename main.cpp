#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include <cstdio>

#include "include/song.hpp"
#include "sound/sound.hpp"

void displayUsage()
{
    std::cout << "Usage: mplay file instrument" << std::endl;
};

int main(int argc, char** argv)
{
    // Check we have the right amount of arguments.
    if (argc != 3)
    {
        displayUsage();
        return -1;
    }

    // If we do, look for the file and write it into a buffer.
    std::ifstream file(argv[1], std::ios::binary);
    
    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return -1;
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

    int i = std::stoi(argv[2]);
    std::cout << "Now playing instrument " << i << " from " << song.name() << std::endl;

    Sound::init(8192);

    
    for (auto& byte : songData)
    {
        uint8_t after = byte + 128;

        byte = reinterpret_cast<uint8_t>(after);
    }    

    Sound::playRaw(reinterpret_cast<const int8_t *>(&(*song.m_samples[i].sampleData)), song.m_samples[i].length);

   // Sound::playRaw(reinterpret_cast<const int8_t *>(songData.data()), songData.size());

    return 0;
}
