#pragma once
#include "input/hotkeys.hpp"
#include <unordered_map>

class HotkeyManager {
public:   
    Hotkey openWheelHotkey;
    
    /**
     * @brief Sets the hotkey to open the wheel
     * @return if it was successfully set or not
     */
    bool SetOpenWheelHotkey(Hotkey hotkey);
};  