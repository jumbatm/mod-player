#include "include/song.hpp"
#include <numeric>
#include <iterator>

Song::Song(std::vector<int8_t>& songData)
{
    // Use an iterator so that we don't have to continually keep track of where we are
    // in the song data.
    std::vector<int8_t>::iterator it = songData.begin();

    // Read the title of the song.
    m_name = std::string(it, it + 20);
    it += 20; // Can't put this in function call, as functions calls are evaluated right to left.
    
    // Read the song's description.
    m_description = std::string(it, it + 21);
    it += 21;
    
}
