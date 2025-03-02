#include "AudioPlayerAsync.h"


AudioPlayerAsync::AudioPlayerAsync() : worker(&AudioPlayerAsync::workerThread, this) {}

AudioPlayerAsync::~AudioPlayerAsync() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
        cv.notify_all();
    }
    worker.join();
}

void AudioPlayerAsync::resetAsync() {
    std::unique_lock<std::mutex> lock(mtx);
    taskQueue.push([this]() {
        //audioPlayerMutex.lock();
        audioPlayer.reset();
        audioPlayer = std::make_unique<AudioPlayer>();
        //audioPlayerMutex.unlock();
        });
    cv.notify_one();
}

void AudioPlayerAsync::playSoundAsync(std::string soundName) {
    
    soundNameMutex.lock();
    latestSoundName = soundName;
    soundNameMutex.unlock();

    std::unique_lock<std::mutex> lock(mtx);
    taskQueue.push([this]() {
        //audioPlayerMutex.lock();
        if (audioPlayer) {
            audioPlayer->playSound(latestSoundName);
        }
        //audioPlayerMutex.unlock();
        });
    cv.notify_one();
}

void AudioPlayerAsync::playMusicAsync(std::string musicName) {

    musicNameMutex.lock();
    latestMusicName = musicName;
    musicNameMutex.unlock();

    std::unique_lock<std::mutex> lock(mtx);
    taskQueue.push([this]() {
        //audioPlayerMutex.lock();
        if (audioPlayer) {
            audioPlayer->playMusic(latestMusicName);
        }
        //audioPlayerMutex.unlock();
        });
    cv.notify_one();
}

void AudioPlayerAsync::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return !taskQueue.empty() || stop; });

            if (stop && taskQueue.empty()) {
                break;
            }

            task = taskQueue.front();
            taskQueue.pop();
        }

        task();
    }
}

