g++ Game.cpp main.cpp Math.cpp OpenGlExtensions.cpp Physics.cpp Renderer.cpp \
    ShaderManager.cpp TextureManager.cpp Utils.cpp BoneAnimatedModel.cpp \
    ObjLoader.cpp cmakeaudioplayer/src/AudioPlayer.cpp TextModel.cpp \
    Inventory.cpp Environment.cpp GameObjectManager.cpp RenderSystem.cpp \
    -o sdl_app -O2 -std=c++17 \
    -I cmakeaudioplayer/include \
    $(pkg-config --cflags --libs sdl2 gl) \
    $(pkg-config --cflags --libs vorbis vorbisfile ogg) \
    -lopenal

#./sdl_app