# Script to run:

```
emcc main.cpp Game.cpp Math.cpp Physics.cpp Renderer.cpp ShaderManager.cpp TextureManager.cpp Utils.cpp OpenGlExtensions.cpp -O2 -std=c++14 -sTOTAL_MEMORY=33554432 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS="[""png""]" -sUSE_SDL=2 --preload-file background.bmp --preload-file bird.bmp32 --preload-file default.fragment --preload-file default.vertex --preload-file game_over.bmp32 --preload-file pipe.bmp32 -o jumpingbird.html
```