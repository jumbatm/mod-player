cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

project(mplay VERSION 0.1
    LANGUAGES CXX)

add_executable(mplay
    main.cpp
    mod/notemixer.cpp
    mod/sample.cpp
    mod/song.cpp
    sound/mixer.cpp
    sound/sound.cpp
    )

target_include_directories(mplay
    PRIVATE ./
    )

find_library(PULSEAUDIO_SIMPLE_LOC pulse-simple)
find_library(PULSEAUDIO_LOC pulse)

target_link_libraries(mplay 
    PRIVATE "${PULSEAUDIO_LOC}"
    PRIVATE "${PULSEAUDIO_SIMPLE_LOC}"
    )