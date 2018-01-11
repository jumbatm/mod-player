#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "mod/song.hpp"
#include "sound/sound.hpp"
#include "sound/mixer.hpp"

void displayUsage()
{
    std::cout << "Usage: mplay file instrument1 instrument2" << std::endl;
};

int main(int argc, char** argv)
{
    // Check we have the right amount of arguments.
    if (argc != 4)
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
    int j = std::stoi(argv[3]);

    std::cout << "Now playing instruments " << i << " and " << j << " from " << song.name() << std::endl;

    Sound::init(8192); 
    // Tentatively the highest frequency needed to play the highest note at the highest finetune: 32842

    
    for (auto& byte : songData) // TODO: Only convert the bytes we need to.
    {
        byte = static_cast<uint8_t>( reinterpret_cast<int8_t&>(byte) + 128 );
    }

    Mixer::newTrack(std::max(song.m_samples[i].length, song.m_samples[j].length));

    Mixer::mixIn(0, &(*song.m_samples[i].sampleData), song.m_samples[i].length);
    Mixer::mixIn(0, &(*song.m_samples[j].sampleData), song.m_samples[j].length);

    Mixer::TrackInfo t = Mixer::getTrack();

    Sound::playRaw(t.data, t.size);


    return 0;
}
