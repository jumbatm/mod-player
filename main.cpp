#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

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
    std::cout << argv[1] << std::endl;
    
    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return -1;
    }

    // Read the data into a byte-sized vector.
    std::vector<int8_t> songData(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

    // Construct a new song object from this data.
    //Song song(songData);
    
    return 0;
}
