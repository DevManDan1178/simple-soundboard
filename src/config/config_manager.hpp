#pragma once
#include "soundboard/soundboard.hpp"
#include "input/hotkeys.hpp"
#include <nlohmann/json.hpp>

class ConfigManager {
public:
    void WriteToJSON() const;
    ConfigManager(Soundboard& soundboard);
private:
    Soundboard& soundboard;
    
    /**
     * @brief updates the soundboard by reading from a JSON file
     */
    void UpdateSoundboardFromJSON();
    
    /**
     * @brief Converts Audio to nlohmann JSON
     * @param audio audio struct
     * @return nlohmann JSON of audio
     */
    nlohmann::json AudioToJSON(const Audio& audio) const;
    
    /**
     * @brief Converts nlohmann JSON into audio
     * @param json JSONified audio struct
     * @return audio struct from the json
     */
    Audio JSONToAudio(const nlohmann::json& json) const;

    /**
     * @brief Converts hotkey to nlohmann JSON
     * @param hotkey hotkey struct
     * @return nlohmann JSON of hotkey
     */
    nlohmann::json HotkeyToJSON(const Hotkey& hotkey) const;

    /**
     * @brief Converts nlohmann JSON into hotkey
     * @param json JSONified hotkey struct
     * @return hotkey struct from the json
     */
    Hotkey JSONToHotkey(const nlohmann::json& json) const;
};