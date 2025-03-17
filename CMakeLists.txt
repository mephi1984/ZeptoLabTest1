cmake_minimum_required(VERSION 3.10)

# Название проекта
project(SDLApp VERSION 1.0 LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Активируем многопоточность
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Находим библиотеки с помощью pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GL REQUIRED gl)

option(AUDIO "Enable audio support" ON)

if(AUDIO)
    pkg_check_modules(VORBIS REQUIRED vorbis vorbisfile ogg)
    pkg_check_modules(OPENAL REQUIRED openal)
    add_definitions(-DAUDIO)  
endif()

# Определяем исполняемый файл и исходники
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
)

if(AUDIO)
    target_link_libraries(sdl_app 
        ${VORBIS_LIBRARIES}
        ${OPENAL_LIBRARIES}
    )
endif()