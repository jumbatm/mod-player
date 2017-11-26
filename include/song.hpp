#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Song
{
private:
   std::string m_name;
   std::string m_description;

public:
    // Constructor.
    Song(std::vector<int8_t>& songData);

    /* Getter methods. */
    const std::string& name() const
    {
        return m_name;
    }

    const std::string& description() const
    {
        return m_description;
    }
};
