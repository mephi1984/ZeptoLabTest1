#pragma once

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <cstdint>

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();

    // Для музыки с зацикливанием (если filename пустой - продолжает играть текущую)
    bool playMusic(const std::string& filename = "");
    
    // Для одноразовых звуковых эффектов
    bool playSound(const std::string& filename);
    
    void stop();
    bool isPlaying() const;

private:
    ALCdevice* device;
    ALCcontext* context;
    ALuint musicSource;  // Источник для музыки
    ALuint soundSource;  // Источник для звуков
    ALuint musicBuffer;  // Буфер для музыки
    ALuint soundBuffer;  // Буфер для звуков
    bool playing;
    std::string currentMusic;  // Хранит имя текущего музыкального файла

    std::vector<char> loadOgg(const std::string& filename, ALuint buffer);
    std::string findFileInSounds(const std::string& filename);
    bool isOggFile(const std::string& filename) const;
};
