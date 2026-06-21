#include "audio_manager.hpp"
#include "audio.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

std::string getAudioFilePath(Audio audio) {
    return audio.audioPath.u8string();
}

AudioManager::AudioManager() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Error initializing MiniAudio engine" << std::endl;
    };
};

AudioManager::~AudioManager() {
    ma_engine_uninit(&engine);
}

bool AudioManager::LoadAudio(Audio audio) {
    std::string audioFilePath = getAudioFilePath(audio);
    ma_sound& loadedAudio = AudioManager::loadedAudios[audioFilePath];

    if (ma_sound_init_from_file(&engine, audioFilePath.c_str(), 0, NULL, NULL, &loadedAudio) != MA_SUCCESS) {
        std::cout << "Failed to load audio: " << audioFilePath << std::endl;
        return false;
    }
    std::cout << "Loaded audio: " << audioFilePath << std::endl;
    return true;
}

void AudioManager::PlayMASound(ma_sound& sound, float volume) {
    ma_sound_set_volume(&sound, volume);
    ma_sound_seek_to_pcm_frame(&sound, 0);
    ma_sound_start(&sound);
}

bool AudioManager::PlayAudio(Audio audio) {
    std::string audioFilePath = getAudioFilePath(audio);

    if (AudioManager::loadedAudios.find(audioFilePath) == AudioManager::loadedAudios.end()) {
        bool loadSuccess = LoadAudio(audio);
        if (!loadSuccess) {
            return false;
        }
    }
    ma_sound& loadedAudio = AudioManager::loadedAudios[audioFilePath];
    std::cout << "Playing audio: " << audioFilePath << std::endl;
    PlayMASound(loadedAudio, 1.0f); //audio.volume
    return true;
}