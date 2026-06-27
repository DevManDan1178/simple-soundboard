#include "config/config_manager.hpp"
#include "event/event_dispatcher.hpp"
#include <nlohmann/json.hpp>
#include <windows.h>
#include "miniaudio.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

constexpr const char* PATH_KEY = "path";
constexpr const char* VOLUME_KEY = "volume";
constexpr const char* NAME_KEY = "name";

constexpr const char* MASTER_VOLUME_KEY = "masterVolume";
constexpr const char* SPEAKER_VOLUME_KEY = "speakerVolume";
constexpr const char* AUDIO_TABLE_KEY = "audioTable";
constexpr const char* OPEN_WHEEL_KEYBIND_KEY = "openWheelKeybind";
constexpr const char* STOP_ALL_AUDIO_KEYBIND_KEY = "stopAllAudioKeybind";
constexpr const char* AUDIO_DRIVER_NAME_KEY = "audioDriverName";

static std::filesystem::path getExeDir()
{
    std::wstring buffer(32767, L'\0');

    DWORD len = GetModuleFileNameW(
        NULL,
        buffer.data(),
        static_cast<DWORD>(buffer.size())
    );

    if (len == 0)
    {
        return std::filesystem::current_path(); // fallback
    }

    buffer.resize(len);

    return std::filesystem::path(buffer).parent_path();
}

static std::filesystem::path getConfigPath()
{
    return getExeDir() / "config" / "config.json";
}

void ConfigManager::WriteToJSON() const {
    try {
        std::filesystem::create_directories(getConfigPath().parent_path());
        std::ofstream file(getConfigPath());
        if (!file.is_open()) {
            std::cout << "Failed to open file for writing: " << getConfigPath() << std::endl;
            return;
        }

        AudioManager& audioManager = soundboard.audioManager;
        AudioTable& audioTable = soundboard.audioTable;
        HotkeyManager& hotkeyManager = soundboard.hotkeyManager;

        json jsonData;

        jsonData[MASTER_VOLUME_KEY] = audioManager.masterVolume;
        jsonData[SPEAKER_VOLUME_KEY] = audioManager.speakerVolumeModifier;
        jsonData[AUDIO_DRIVER_NAME_KEY] = audioManager.GetCurrentAudioDriverName();
        std::cout << "saved audio driver name " << audioManager.GetCurrentAudioDriverName() << std::endl;

        jsonData[OPEN_WHEEL_KEYBIND_KEY] = HotkeyToJSON(hotkeyManager.openWheelHotkey);
        jsonData[STOP_ALL_AUDIO_KEYBIND_KEY] = HotkeyToJSON(hotkeyManager.stopAllAudioHotkey);

        for (int tableIdx = 0; tableIdx < audioTable.size(); tableIdx++) {
            std::vector<Audio>& audios = *audioTable.GetAudiosAtIdx(tableIdx);
            for (int audioIdx = 0; audioIdx < audios.size(); audioIdx++) {
                jsonData[AUDIO_TABLE_KEY][tableIdx][audioIdx] = AudioToJSON(audios[audioIdx]);
            }
        }

        file << jsonData.dump(4);
        EventDispatcher::Emit(ConfigSavedEvent());
    } catch (const std::exception& e) {
        std::cout << "Error writing to JSON: " << e.what() << std::endl;
    }
}

void ConfigManager::UpdateSoundboardFromJSON() {
    try {
        std::filesystem::create_directories(getConfigPath().parent_path());
        std::ifstream file(getConfigPath());
        if (!file.is_open()) {
            std::cout << "Failed to open file at path " << getConfigPath() << std::endl;
            return;
        }
        AudioManager& audioManager = soundboard.audioManager;
        AudioTable& audioTable = soundboard.audioTable;
        HotkeyManager& hotkeyManager = soundboard.hotkeyManager;

        json jsonData;
        file >> jsonData;

        audioManager.masterVolume = jsonData[MASTER_VOLUME_KEY].get<float>();
        audioManager.speakerVolumeModifier= jsonData[SPEAKER_VOLUME_KEY].get<float>();
        hotkeyManager.SetOpenWheelHotkey(JSONToHotkey(jsonData[OPEN_WHEEL_KEYBIND_KEY]));
        hotkeyManager.SetStopAllAudioHotkey(JSONToHotkey(jsonData[STOP_ALL_AUDIO_KEYBIND_KEY]));

        std::string audioDriverName = jsonData[AUDIO_DRIVER_NAME_KEY].get<std::string>();
        std::vector<ma_device_info> availableDrivers = audioManager.GetAvailableDrivers();

        for (int i = 0; i < availableDrivers.size(); i++) {
            if (availableDrivers[i].name == audioDriverName) {
                audioManager.SetAudioDriver(i);
                std::cout << availableDrivers[i].name << " - " << audioDriverName << std::endl;
                break;
            }
        }

        audioTable.Clear();
        for (int tableIdx = 0; tableIdx < jsonData[AUDIO_TABLE_KEY].size(); tableIdx++) {
            audioTable.CreateSubTable();
            auto& audioList = jsonData[AUDIO_TABLE_KEY][tableIdx];
            for (int audioIdx = 0; audioIdx < audioList.size(); audioIdx++) {
                Audio audio = JSONToAudio(audioList[audioIdx]);
                audioTable.AddAudio(audio, tableIdx);
            }
        }

    }   catch (const std::exception& e) {
        std::cout << "Error reading from JSON: " << e.what() << std::endl;
    }
}


ConfigManager::ConfigManager(Soundboard& sb) : soundboard(sb) {
    UpdateSoundboardFromJSON();
    EventDispatcher::Subscribe([this](const Event& event) -> void {
        if (event.getType() == EventType::ConfigChange) {
            WriteToJSON();
        }
    });
}

json ConfigManager::AudioToJSON(const Audio &audio) const {
    return {
        {PATH_KEY, audio.filePath.string()},
        {NAME_KEY, audio.name},
        {VOLUME_KEY, audio.volume},
    };
}

Audio ConfigManager::JSONToAudio(const json &json) const {
    std::filesystem::path path(json[PATH_KEY].get<std::string>());
    float volume = json[VOLUME_KEY].get<float>();
    std::string name = json[NAME_KEY].get<std::string>();
    return Audio(path, volume, name);
}

nlohmann::json ConfigManager::HotkeyToJSON(const Hotkey& hotkey) const {
    json jsonData;
    jsonData["keyCode"] = hotkey.keyCode;
    jsonData["modifiers"] = hotkey.modifiers;
    jsonData["bindType"] = hotkey.bindType == BindType::Keyboard ? "Keyboard" : (hotkey.bindType == BindType::Mouse ? "Mouse" : "None");
    return jsonData;
}


Hotkey ConfigManager::JSONToHotkey(const nlohmann::json& json) const {
    Hotkey hotkey;
    hotkey.keyCode = json["keyCode"].get<int>();
    hotkey.modifiers = Modifiers{json["modifiers"].get<unsigned int>()};
    std::string bindType = json["bindType"].get<std::string>();
    hotkey.bindType = bindType == "Keyboard" ? BindType::Keyboard : (bindType == "Mouse" ? BindType::Mouse : BindType::None);
    return hotkey;
}
