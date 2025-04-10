# Script to run:

```
C:\Work\Projects\emsdk\emsdk.bat activate latest
C:\Work\Projects\emsdk\emsdk_env.bat
emcc main.cpp Game.cpp Math.cpp Physics.cpp Renderer.cpp ShaderManager.cpp TextureManager.cpp Utils.cpp OpenGlExtensions.cpp -O2 -std=c++14 -sTOTAL_MEMORY=33554432 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS="[""png""]" -sUSE_SDL=2 --preload-file background.bmp --preload-file bird.bmp32 --preload-file default.fragment --preload-file default.vertex --preload-file game_over.bmp32 --preload-file pipe.bmp32 -o jumpingbird.html
```

```


zlib-1.3.1:

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=install ..

then run ALL_BUILD and INSTALL in Visual Studio

lpng1645:

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=install -DZLIB_ROOT=C:\Work\Projects\zlib-1.3.1\build\install ..

then run ALL_BUILD and INSTALL in Visual Studio


```


https://github.com/Bly7/OBJ-Loader/blob/master/Source/OBJ_Loader.h


https://github.com/gametutorials/tutorials/blob/master/OpenGL/MD3%20Animation/Main.cpp



linux:
```
g++ Game.cpp main.cpp Math.cpp OpenGlExtensions.cpp Physics.cpp Renderer.cpp ShaderManager.cpp TextureManager.cpp Utils.cpp BoneAnimatedModel.cpp ObjLoader.cpp cmakeaudioplayer/src/AudioPlayer.cpp TextModel.cpp Inventory.cpp -o sdl_app -O2 -std=c++17 \
-I cmakeaudioplayer/include \
$(pkg-config --cflags --libs sdl2 gl) \
$(pkg-config --cflags --libs vorbis vorbisfile ogg) \
-lopenal
```

# Emscripten new

```

cd build-emcmake/

emcmake cmake ..

 emcmake cmake -DCMAKE_INSTALL_PREFIX=install ..
 cmake --build .

 cmake --install .


emcc main.cpp Game.cpp Environment.cpp GameObjectManager.cpp BoneAnimatedModel.cpp GameWorld.cpp InputManager.cpp Inventory.cpp ObjLoader.cpp QuestScripts.cpp RenderSystem.cpp  Math.cpp Physics.cpp Renderer.cpp TextModel.cpp  ShaderManager.cpp TextureManager.cpp Utils.cpp OpenGlExtensions.cpp -O2 -std=c++14 -IC:\Users\ASUS\Desktop\fishrungame2\ZeptoLabTest1\thirdparty\libzip-1.11.3\build-emcmake\install\include -LC:\Users\ASUS\Desktop\fishrungame2\ZeptoLabTest1\thirdparty\libzip-1.11.3\build-emcmake\install\lib -lzip -sTOTAL_MEMORY=33554432 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS="[""png""]" -sUSE_SDL=2 --preload-file background.bmp --preload-file bird.bmp32 --preload-file default.fragment --preload-file default.vertex --preload-file game_over.bmp32 --preload-file pipe.bmp32 -o jumpingbird.html

emcc main.cpp Game.cpp Environment.cpp GameObjectManager.cpp BoneAnimatedModel.cpp GameWorld.cpp InputManager.cpp Inventory.cpp ObjLoader.cpp QuestScripts.cpp RenderSystem.cpp Math.cpp Physics.cpp Renderer.cpp TextModel.cpp ShaderManager.cpp TextureManager.cpp Utils.cpp OpenGlExtensions.cpp -O2 -std=c++14 -pthread -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4 -sTOTAL_MEMORY=4294967296 -sINITIAL_MEMORY=3221225472 -sMAXIMUM_MEMORY=4294967296 -sALLOW_MEMORY_GROWTH=1 -I./thirdparty/libzip-1.11.3/build-emcmake/install/include -I./thirdparty/zlib-1.3.1/install/include -L./thirdparty/libzip-1.11.3/build-emcmake/install/lib -L./thirdparty/zlib-1.3.1/install/lib -lzip -lz -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL=2 --preload-file data.zip -o jumpingbird.html


# Windows:
emcc main.cpp Game.cpp Environment.cpp GameObjectManager.cpp BoneAnimatedModel.cpp GameWorld.cpp InputManager.cpp Inventory.cpp ObjLoader.cpp QuestScripts.cpp RenderSystem.cpp Math.cpp Physics.cpp Renderer.cpp TextModel.cpp ShaderManager.cpp TextureManager.cpp Utils.cpp OpenGlExtensions.cpp -O2 -std=c++14 -pthread -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4 -sTOTAL_MEMORY=4294967296 -sINITIAL_MEMORY=3221225472 -sMAXIMUM_MEMORY=4294967296 -sALLOW_MEMORY_GROWTH=1 -I./thirdparty/libzip-1.11.3/build-emcmake/install/include -L./thirdparty/libzip-1.11.3/build-emcmake/install/lib -lzip -lz -sUSE_SDL_IMAGE=2 -sUSE_SDL=2 --preload-file data.zip -o viola.html

emrun --no_browser --port 8080 .
```

# License
Code: MIT

Art: CC-BY

# Cmake Run Linux
Run using cmakelist
make -j$(nproc) -C build #Компилируем
./build/sdl_app #Запускаем

Для постройки без звука
rm -rf build  #Очищаем build папку
cmake -B build -DAUDIO=1 #Пересоздаём конфигурацию CMake