#include "AudioPlayer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

AudioPlayer::AudioPlayer() : device(nullptr), context(nullptr), 
    musicSource(0), soundSource(0), musicBuffer(0), soundBuffer(0), playing(false) {
    device = alcOpenDevice(nullptr);
    if (!device) {
        throw std::runtime_error("Failed to open audio device");
    }

    context = alcCreateContext(device, nullptr);
    if (!context) {
        alcCloseDevice(device);
        throw std::runtime_error("Failed to create audio context");
    }

    alcMakeContextCurrent(context);
    alGenSources(1, &musicSource);
    alGenSources(1, &soundSource);
    alGenBuffers(1, &musicBuffer);
    alGenBuffers(1, &soundBuffer);
}

AudioPlayer::~AudioPlayer() {
    if (musicSource)
        alDeleteSources(1, &musicSource);
    if (soundSource)
        alDeleteSources(1, &soundSource);
    if (musicBuffer)
        alDeleteBuffers(1, &musicBuffer);
    if (soundBuffer)
        alDeleteBuffers(1, &soundBuffer);
    
    if (context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
    }
    if (device)
        alcCloseDevice(device);
}

bool AudioPlayer::isOggFile(const std::string& filename) const {
    std::string ext = std::filesystem::path(filename).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".ogg";
}

std::string AudioPlayer::findFileInSounds(const std::string& filename) {
    // Primary search path - "sounds" directory next to executable
    std::filesystem::path soundsDir = std::filesystem::current_path() / "sounds";
    
    // Alternative search paths
    std::vector<std::filesystem::path> altPaths = {
        std::filesystem::current_path() / ".." / "sounds",          // One level up
        std::filesystem::current_path() / ".." / ".." / "sounds",   // Two levels up
        "/home/albert/gay-jam/ZeptoLabTest1/sounds"                 // Absolute path
    };
    
    std::cout << "🔍 Searching for \"" << filename << "\" in:\n";
    std::cout << "   " << soundsDir << "\n";
    
    if (std::filesystem::exists(soundsDir / filename)) {
        return (soundsDir / filename).string();
    }
    
    // Try alternative paths
    for (const auto& path : altPaths) {
        std::cout << "   " << path << "\n";
        if (std::filesystem::exists(path / filename)) {
            return (path / filename).string();
        }
    }
    
    throw std::runtime_error("❌ File not found: " + filename);
}

std::vector<char> AudioPlayer::loadOgg(const std::string& filename, ALuint buffer) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    OggVorbis_File vf;
    if (ov_open_callbacks(file, &vf, nullptr, 0, OV_CALLBACKS_DEFAULT) < 0) {
        fclose(file);
        throw std::runtime_error("Input not an Ogg file: " + filename);
    }

    vorbis_info* vi = ov_info(&vf, -1);
    std::vector<char> audioData;
    char data[4096];
    int bitstream;
    long bytes;

    do {
        bytes = ov_read(&vf, data, sizeof(data), 0, 2, 1, &bitstream);
        if (bytes > 0) {
            audioData.insert(audioData.end(), data, data + bytes);
        }
    } while (bytes > 0);

    // Setup the buffer with the audio data
    alBufferData(buffer, 
                 (vi->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
                 audioData.data(),
                 audioData.size(),
                 vi->rate);

    ov_clear(&vf);
    return audioData;
}

bool AudioPlayer::playMusic(const std::string& filename) {
    try {
        // Если filename пустой, просто проверяем играет ли музыка
        if (filename.empty()) {
            if (!isPlaying()) {
                alSourcei(musicSource, AL_LOOPING, AL_TRUE);  // Включаем зацикливание
                alSourcePlay(musicSource);
            }
            return true;
        }

        // Если filename не пустой, загружаем новую музыку
        if (!isOggFile(filename)) {
            std::cerr << "❌ Error: Music file must be an .ogg file\n";
            return false;
        }

        std::string fullPath = findFileInSounds(filename);
        std::cout << "✅ Found music file: " << fullPath << "\n";
        
        // Останавливаем текущую музыку
        alSourceStop(musicSource);
        
        // Загружаем и настраиваем новую музыку
        loadOgg(fullPath, musicBuffer);
        alSourcei(musicSource, AL_BUFFER, musicBuffer);
        alSourcei(musicSource, AL_LOOPING, AL_TRUE);  // Включаем зацикливание
        
        std::cout << "▶️ Starting music playback...\n";
        alSourcePlay(musicSource);
        
        currentMusic = filename;
        playing = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error playing music: " << e.what() << std::endl;
        return false;
    }
}

bool AudioPlayer::playSound(const std::string& filename) {
    try {
        if (!isOggFile(filename)) {
            std::cerr << "❌ Error: Sound file must be an .ogg file\n";
            return false;
        }

        std::string fullPath = findFileInSounds(filename);
        std::cout << "✅ Found sound file: " << fullPath << "\n";
        
        // Загружаем и настраиваем звук
        loadOgg(fullPath, soundBuffer);
        alSourcei(soundSource, AL_BUFFER, soundBuffer);
        alSourcei(soundSource, AL_LOOPING, AL_FALSE);  // Выключаем зацикливание
        
        std::cout << "▶️ Playing sound effect...\n";
        alSourcePlay(soundSource);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error playing sound: " << e.what() << std::endl;
        return false;
    }
}

void AudioPlayer::stop() {
    alSourceStop(musicSource);
    alSourceStop(soundSource);
    playing = false;
}

bool AudioPlayer::isPlaying() const {
    ALint state;
    alGetSourcei(musicSource, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}
