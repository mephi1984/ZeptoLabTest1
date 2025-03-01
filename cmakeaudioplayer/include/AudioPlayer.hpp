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

    bool playFromSoundsDir(const std::string& filename);
    void stop();
    bool isPlaying() const;

private:
    ALCdevice* device;
    ALCcontext* context;
    ALuint source;
    ALuint buffer;
    bool playing;

    std::vector<char> loadOgg(const std::string& filename);
    std::string findFileInSounds(const std::string& filename);
    bool isOggFile(const std::string& filename) const;
};
