cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build unit tests" ON)

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

target_compile_options(mplay
    PRIVATE
    -Wall
    -Wextra
    #-O2
    -g
    )

find_library(PULSEAUDIO_SIMPLE_LOC pulse-simple)
find_library(PULSEAUDIO_LOC pulse)

target_link_libraries(mplay 
    PRIVATE "${PULSEAUDIO_LOC}"
    PRIVATE "${PULSEAUDIO_SIMPLE_LOC}"
    )

if (BUILD_TESTS)
    enable_testing()

    add_executable(Test
        tests/main_test.cpp
        tests/note_test.cpp
        )

    target_include_directories(Test
        PRIVATE
        extern/catch-mini/
        ./
        )
    target_link_libraries(Test
        PRIVATE "${PULSEAUDIO_LOC}"
        PRIVATE "${PULSEAUDIO_SIMPLE_LOC}"
        )

    target_compile_options(Test
        PUBLIC
        -g
        -Dprivate=public
        -Wall
        -Wextra
        )

    add_test(
        NAME Test
        COMMAND Test
        ) 
endif()
