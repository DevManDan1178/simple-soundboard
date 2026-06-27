#include "audio_manager.hpp"
#include "event/event_dispatcher.hpp"
#include "audio.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

FilePath AudioManager::getAudioFilePath(const Audio& audio) {
    return audio.filePath.string();
}

AudioManager::AudioManager()
{
    ma_engine_config engineConfig = ma_engine_config_init();
    engineConfig.sampleRate = 48000;

    if (ma_engine_init(&engineConfig, &systemEngine) != MA_SUCCESS)
        std::cout << "[AudioManager] ERROR: Failed to init systemEngine\n";

    if (ma_engine_init(&engineConfig, &deviceEngine) != MA_SUCCESS)
        std::cout << "[AudioManager] ERROR: Failed to init deviceEngine\n";
}

void AudioManager::StopAllAudio()
{
    for (auto& [path, audio] : loadedAudios)
    {
        ma_sound_stop(&audio.speaker);
        ma_sound_stop(&audio.virtualDevice);
        ma_sound_seek_to_pcm_frame(&audio.speaker, 0);
        ma_sound_seek_to_pcm_frame(&audio.virtualDevice, 0);
    }
}

AudioManager::~AudioManager()
{
    UnloadAllAudio();
    ma_engine_uninit(&deviceEngine);
    ma_engine_uninit(&systemEngine);
}

void AudioManager::UnloadAllAudio()
{
    for (auto& [path, audio] : loadedAudios)
    {
        ma_sound_uninit(&audio.speaker);
        ma_sound_uninit(&audio.virtualDevice);
    }
    loadedAudios.clear();
}

bool AudioManager::LoadAudio(const Audio& audio)
{
    FilePath path = audio.filePath.string();

    LoadedAudio& loaded = loadedAudios[path];

    if (ma_sound_init_from_file(
        &systemEngine,
        path.c_str(),
        0, NULL, NULL,
        &loaded.speaker) != MA_SUCCESS)
    {
        std::cout << "[LoadAudio] ERROR: System sound load failed: " << path << "\n";
        return false;
    }

    if (ma_sound_init_from_file(
        &deviceEngine,
        path.c_str(),
        0, NULL, NULL,
        &loaded.virtualDevice) != MA_SUCCESS)
    {
        std::cout << "[LoadAudio] ERROR: Device sound load failed: " << path << "\n";
        return false;
    }

    ma_sound_set_looping(&loaded.speaker, MA_FALSE);
    ma_sound_set_looping(&loaded.virtualDevice, MA_FALSE);
    return true;
}

std::vector<ma_device_info> AudioManager::GetAvailableDrivers()
{
    std::vector<ma_device_info> drivers;

    ma_context_config config = ma_context_config_init();
    ma_context context;

    ma_backend backends[] = {
        ma_backend_wasapi,
        ma_backend_dsound,
        ma_backend_winmm
    };

    if (ma_context_init(backends, 3, &config, &context) != MA_SUCCESS)
    {
        std::cout << "[GetAvailableDrivers] ERROR: Failed to init context\n";
        return drivers;
    }

    ma_device_info* pPlaybackInfos = nullptr;
    ma_uint32 count = 0;

    if (ma_context_get_devices(&context, &pPlaybackInfos, &count, NULL, NULL) != MA_SUCCESS)
    {
        std::cout << "[GetAvailableDrivers] ERROR: Failed to get devices\n";
        ma_context_uninit(&context);
        return drivers;
    }

    playbackDevices.assign(pPlaybackInfos, pPlaybackInfos + count);
    selectedDeviceID = nullptr;

    ma_context_uninit(&context);
    return playbackDevices;
}

std::string AudioManager::GetCurrentAudioDriverName() {
    return audioDriverName;
}

void AudioManager::SetAudioDriver(unsigned int index)
{
    if (index >= playbackDevices.size())
    {
        std::cout << "[SetAudioDriver] ERROR: Invalid device index\n";
        return;
    }
    if (selectedDeviceID == &playbackDevices[index].id) {
        return;
    }

    std::cout << "audio driver changed" << std::endl;
    selectedDeviceID = &playbackDevices[index].id;
    audioDriverName = playbackDevices[index].name;
    selectedDriverIdx = index;
    EventDispatcher::Emit(ConfigChangeEvent());

    // Uninit all sounds bound to the old deviceEngine before destroying it
    for (auto& [path, audio] : loadedAudios)
        ma_sound_uninit(&audio.virtualDevice);

    ma_engine_uninit(&deviceEngine);

    // Reinit deviceEngine with the new device, preserving sample rate
    ma_engine_config config = ma_engine_config_init();
    config.pPlaybackDeviceID = selectedDeviceID;
    config.sampleRate = 48000;

    if (ma_engine_init(&config, &deviceEngine) != MA_SUCCESS)
    {
        std::cout << "[SetAudioDriver] ERROR: Failed to init deviceEngine\n";
        selectedDeviceID = nullptr;
        return;
    }

    // Reload all virtualDevice sounds into the new engine
    for (auto& [path, audio] : loadedAudios)
    {
        if (ma_sound_init_from_file(
            &deviceEngine,
            path.c_str(),
            0, NULL, NULL,
            &audio.virtualDevice) != MA_SUCCESS)
        {
            std::cout << "[SetAudioDriver] ERROR: Failed to reload sound: " << path << "\n";
        }
        ma_sound_set_looping(&audio.virtualDevice, MA_FALSE);
    }

    std::cout << "[SetAudioDriver] OK: deviceEngine set to: "
              << playbackDevices[index].name << "\n";

}

void AudioManager::PlayMALoadedAudio(LoadedAudio& audio, const float volume, bool ignoreVirtualDevice)
{
    ma_sound_set_volume(&audio.speaker, volume * speakerVolumeModifier);
    ma_sound_seek_to_pcm_frame(&audio.speaker, 0);
    ma_sound_start(&audio.speaker);
    if (ignoreVirtualDevice) {
        return;
    }
    ma_sound_set_volume(&audio.virtualDevice, volume);
    ma_sound_seek_to_pcm_frame(&audio.virtualDevice, 0);
    ma_sound_start(&audio.virtualDevice);
}

bool AudioManager::PlayAudio(const Audio& audio, bool ignoreVirtualDevice)
{
    FilePath audioFilePath = getAudioFilePath(audio);

    if (loadedAudios.find(audioFilePath) == loadedAudios.end())
    {
        if (!LoadAudio(audio))
            return false;
    }

    LoadedAudio& loadedAudio = loadedAudios[audioFilePath];
    PlayMALoadedAudio(loadedAudio, audio.volume * masterVolume, ignoreVirtualDevice);
    return true;
}