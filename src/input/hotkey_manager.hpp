#pragma once
#include "input/hotkeys.hpp"
#include <unordered_map>

class HotkeyManager {
public:

    /**
     * @brief queries for a hotkey input, suspending all other input detection. (Use for changing hotkeys in UI)
     * @return the hotkey obtained
     */
    Hotkey queryHotkey();
    
    /**
     * @brief gets the current state of modifier inputs
     * @return Modifiers object of the current state of modifier inputs
     */
    Modifiers& getModifierState();
};  