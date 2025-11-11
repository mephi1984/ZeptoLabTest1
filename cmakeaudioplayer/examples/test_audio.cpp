#include "AudioPlayer.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

int main() {
    try {
        AudioPlayer player;
        const std::string filename = "Symphony No.6 (1st movement).ogg";
        
        std::cout << "🔍 Looking for file: " << filename << " in sounds directory...\n";
        
        if (!player.playFromSoundsDir(filename)) {
            std::cout << "❌ Failed to play audio file\n";
            return 1;
        }
        
        std::cout << "✅ Playing symphony...\n";

        // Check status for 30 seconds
        for (int i = 0; i < 30; ++i) {
            std::cout << "📊 Status: " << (player.isPlaying() ? "Playing ▶️" : "Stopped ⏹️") << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }
}
