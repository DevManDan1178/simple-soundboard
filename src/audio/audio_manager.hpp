#pragma once
#include "audio.hpp"
#include "miniaudio.h"
#include <unordered_map>
#include <string>
class AudioManager {
public:
    ma_engine engine;

    AudioManager();
    ~AudioManager();
    bool LoadAudio(Audio audio);
    bool PlayAudio(Audio audio);
private:
    std::unordered_map<std::string, ma_sound> loadedAudios;
    void PlayMASound(ma_sound& sound, float volume = 1.0f);
};