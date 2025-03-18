cmake_minimum_required(VERSION 3.10)

project(SDLApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(LIBZIP REQUIRED libzip)

option(AUDIO "Audio support" OFF)

if(AUDIO)
    set(AUDIO ON)
else()
    set(AUDIO OFF)
endif()

if(AUDIO)
    pkg_check_modules(VORBIS REQUIRED vorbis vorbisfile ogg)
    pkg_check_modules(OPENAL REQUIRED openal)
    add_definitions(-DAUDIO)  
endif()

add_executable(sdl_app 
    Game.cpp
    main.cpp
    Math.cpp
    OpenGlExtensions.cpp
    Physics.cpp
    Renderer.cpp
    ShaderManager.cpp
    TextureManager.cpp
    Utils.cpp
    BoneAnimatedModel.cpp
    ObjLoader.cpp
    TextModel.cpp
    Inventory.cpp
    Environment.cpp
    GameObjectManager.cpp
    RenderSystem.cpp
    QuestScripts.cpp
)

if(AUDIO)
    target_sources(sdl_app PRIVATE 
        cmakeaudioplayer/src/AudioPlayer.cpp
        AudioPlayerAsync.cpp
    )
endif()

target_include_directories(sdl_app PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
    ${LIBZIP_INCLUDE_DIRS}
)

if(AUDIO)
    target_include_directories(sdl_app PRIVATE
        ${VORBIS_INCLUDE_DIRS}
        ${OPENAL_INCLUDE_DIRS}
        cmakeaudioplayer/include
    )
endif()

target_link_libraries(sdl_app 
    ${SDL2_LIBRARIES}
    ${GL_LIBRARIES}
    Threads::Threads
    ${LIBZIP_LIBRARIES}
)

if(AUDIO)
    target_link_libraries(sdl_app 
        ${VORBIS_LIBRARIES}
        ${OPENAL_LIBRARIES}
    )
endif()