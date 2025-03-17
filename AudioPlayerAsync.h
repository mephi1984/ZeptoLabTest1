#pragma once

#ifdef AUDIO

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include "cmakeaudioplayer/include/AudioPlayer.hpp"


class AudioPlayerAsync {
public:
    AudioPlayerAsync();
    ~AudioPlayerAsync();

    void resetAsync();

    void playSoundAsync(std::string soundName);

    void playMusicAsync(std::string musicName);

    void stopAsync();

    void exit()
    {
        stop = true;
    }

    std::thread worker;

private:
    std::unique_ptr<AudioPlayer> audioPlayer;
    //std::mutex audioPlayerMutex;

    std::mutex soundNameMutex;
    std::mutex musicNameMutex;

    std::string latestSoundName;
    std::string latestMusicName;

    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::function<void()>> taskQueue;
    bool stop = false;

    void workerThread();

};

#endif 