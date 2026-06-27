#pragma once
#include "config/config_manager.hpp"
#include "soundboard/soundboard.hpp"

namespace ConfigurationUI {
    void Render(Soundboard& soundboard, bool& unsavedAudioChanges);

    void RenderAudioConfigurations(Soundboard& soundboard, bool& unsavedChanges);
    void RenderHotkeyConfigurations(HotkeyManager& hotkeyManager);
    void RenderDriverConfigurations(AudioManager& audioManager);
}