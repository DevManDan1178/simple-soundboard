#pragma once
#include "input/hotkeys.hpp"
#include <unordered_map>
#include "input/input_listener.hpp"

class HotkeyManager {
public:  
    Hotkey openWheelHotkey;
    
    HotkeyManager();
    /**
     * @brief Sets the hotkey to open the wheel
     * @return if it was successfully set or not
     */
    bool SetOpenWheelHotkey(Hotkey hotkey);

};  