#include "audio_manager.hpp"
#include "audio.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

FilePath AudioManager::getAudioFilePath(const Audio& audio) {
    return audio.filePath.string();
}

AudioManager::AudioManager() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Error initializing MiniAudio engine" << std::endl;
    };
};

AudioManager::~AudioManager() {
    ma_engine_uninit(&engine);
}

bool AudioManager::LoadAudio(const Audio& audio) {
    FilePath audioFilePath = getAudioFilePath(audio);
    
    if (loadedAudios.find(audioFilePath) != loadedAudios.end()) {
        std::cout << "Audio is already loaded: " << audioFilePath << std::endl;
        return true;
    }
    ma_sound& loadedAudio = AudioManager::loadedAudios[audioFilePath];

    if (ma_sound_init_from_file(&engine, audioFilePath.c_str(), 0, NULL, NULL, &loadedAudio) != MA_SUCCESS) {
        std::cout << "Failed to load audio: " << audioFilePath << std::endl;
        return false;
    }
    std::cout << "Loaded audio: " << audioFilePath << std::endl;
    return true;
}

void AudioManager::PlayMASound(ma_sound& sound, const float volume) {
    ma_sound_set_volume(&sound, volume);
    ma_sound_seek_to_pcm_frame(&sound, 0);
    ma_sound_start(&sound);
}

bool AudioManager::PlayAudio(const Audio& audio) {
    FilePath audioFilePath = getAudioFilePath(audio);

    if (AudioManager::loadedAudios.find(audioFilePath) == AudioManager::loadedAudios.end()) {
        bool loadSuccess = LoadAudio(audio);
        if (!loadSuccess) {
            return false;
        }
    }
    ma_sound& loadedAudio = AudioManager::loadedAudios[audioFilePath];
    std::cout << "Playing audio: " << audio.name << std::endl;
    PlayMASound(loadedAudio, audio.volume * masterVolume); 
    return true;
}