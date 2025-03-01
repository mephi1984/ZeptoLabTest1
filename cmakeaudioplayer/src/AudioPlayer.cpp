#include "AudioPlayer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

AudioPlayer::AudioPlayer() : device(nullptr), context(nullptr), source(0), buffer(0), playing(false) {
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
    alGenSources(1, &source);
    alGenBuffers(1, &buffer);
}

AudioPlayer::~AudioPlayer() {
    if (source)
        alDeleteSources(1, &source);
    if (buffer)
        alDeleteBuffers(1, &buffer);
    
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
    // Check relative to executable location first (../../sounds)
    std::filesystem::path soundsDir = std::filesystem::current_path() / ".." / ".." / "sounds";
    
    // Fallback to ../sounds if not found
    std::filesystem::path altSoundsDir = std::filesystem::current_path() / ".." / "sounds";
    
    std::cout << "🔍 Searching for \"" << filename << "\" in:\n";
    std::cout << "   " << soundsDir << "\n";
    std::cout << "   " << altSoundsDir << "\n";
    
    if (std::filesystem::exists(soundsDir / filename)) {
        return (soundsDir / filename).string();
    }
    
    if (std::filesystem::exists(altSoundsDir / filename)) {
        return (altSoundsDir / filename).string();
    }
    
    throw std::runtime_error("❌ File not found: " + filename);
}

std::vector<char> AudioPlayer::loadOgg(const std::string& filename) {
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

bool AudioPlayer::playFromSoundsDir(const std::string& filename) {
    try {
        if (!isOggFile(filename)) {
            std::cerr << "❌ Error: File must be an .ogg file\n";
            return false;
        }

        std::string fullPath = findFileInSounds(filename);
        std::cout << "✅ Found file: " << fullPath << "\n";
        
        auto audioData = loadOgg(fullPath);
        alSourcei(source, AL_BUFFER, buffer);
        
        alGetError(); // Clear any previous errors
        
        std::cout << "▶️ Starting playback...\n";
        alSourcePlay(source);
        
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            std::cerr << "❌ OpenAL error: " << error << std::endl;
            return false;
        }
        
        playing = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return false;
    }
}

void AudioPlayer::stop() {
    alSourceStop(source);
    playing = false;
}

bool AudioPlayer::isPlaying() const {
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}
