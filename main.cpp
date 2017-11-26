#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "include/song.hpp"

void displayUsage()
{
    std::cout << "TODO: Document usage." << std::endl;
};

int main(int argc, char** argv)
{
    // Check we have the right amount of arguments.
    if (argc != 2)
    {
        displayUsage();
    }

    // If we do, look for the file and write it into a buffer.
    std::ifstream file(argv[1]);
    
    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return -1;
    }

    // Read the data into a byte-sized vector.
    std::vector<int8_t> songData(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));
    
    // From now on, we just work from the buffer.
    file.close();

    // Construct a new song object from this data.
    Song song(songData);

    std::cout << "Now playing: " << song.name() << std::endl;
    std::cout << song.description() << std::endl;
    
    return 0;
}
