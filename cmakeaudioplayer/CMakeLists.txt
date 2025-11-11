cmake_minimum_required(VERSION 3.10)
project(AudioPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find Vorbis
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(VORBIS REQUIRED vorbis vorbisfile)
#pkg_check_modules(OGG REQUIRED ogg)
find_package(OpenAL REQUIRED)

add_library(audioplayer
    src/AudioPlayer.cpp
    include/AudioPlayer.hpp
)

target_include_directories(audioplayer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENAL_INCLUDE_DIR}
        ${VORBIS_INCLUDE_DIRS}
        ${OGG_INCLUDE_DIRS}
)

target_link_libraries(audioplayer
    PUBLIC
        ${OPENAL_LIBRARY}
        ${VORBIS_LIBRARIES}
        ${OGG_LIBRARIES}
)

# Test executable
add_executable(test_audio examples/test_audio.cpp)
target_link_libraries(test_audio PRIVATE audioplayer stdc++fs)
#git add ../../sounds
