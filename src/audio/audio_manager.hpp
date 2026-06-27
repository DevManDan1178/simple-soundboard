#pragma once
#include "audio.hpp"
#include "miniaudio.h"
#include <unordered_map>
#include <string>
#include <vector>

using FilePath = std::string;

struct LoadedAudio { 
    ma_sound speaker;
    ma_sound virtualDevice;
};

class AudioManager {
public:
    
    float masterVolume = 1.0f;
    float speakerVolumeModifier = 1.0f;

    // Owned copies of device infos — safe after context is destroyed
    std::vector<ma_device_info> playbackDevices;
    // For UI
    int selectedDriverIdx = -1;

    AudioManager();
    ~AudioManager();
    
    /**
     * Stops all audios
     */
    void StopAllAudio();

    /**
     * @brief Loads the audio into both systemEngine and deviceEngine.
     * @param audio the audio to load
     * @return true if successful
     */
    bool LoadAudio(const Audio& audio);

    /**
     * @brief Plays the audio (loads it first if not already cached).
     * @param audio the audio to play
     * @param ignoreVirtualDevice if to only play it on the speakers and not on the virtual device
     * @return true if successful
     */
    bool PlayAudio(const Audio& audio, bool ignoreVirtualDevice = false);

    /**
     * @brief Returns a list of available playback device names.
     *        Must be called before SetAudioDriver.
     */
    std::vector<ma_device_info> GetAvailableDrivers();

    /**
     * @brief Switches deviceEngine to the given playback device.
     *        Automatically re-binds all loaded sounds to the new engine.
     * @param index index into the list returned by GetAvailableDrivers
     */
    void SetAudioDriver(unsigned int index);


    /**
     * Gets the name of the current virtual device
     */
    std::string GetCurrentAudioDriverName();

    FilePath getAudioFilePath(const Audio& audio);

private:
    std::string audioDriverName;
    ma_engine systemEngine{};
    ma_engine deviceEngine{};

    // Points into playbackDevices — valid as long as playbackDevices isn't resized
    ma_device_id* selectedDeviceID = nullptr;

    std::unordered_map<FilePath, LoadedAudio> loadedAudios;

    void OnAudioDeviceChange();
    /**
     * @brief Uninits and clears all loaded sounds from both engines.
     */
    void UnloadAllAudio();

    /**
     * @brief Plays a loaded sound pair at the given volume.
     * @param audio the loaded audio
     * @param volume playback volume (default 1.0)
     */
    void PlayMALoadedAudio(LoadedAudio& audio, const float volume = 1.0f, bool ignoreVirtualDevice = false);
};