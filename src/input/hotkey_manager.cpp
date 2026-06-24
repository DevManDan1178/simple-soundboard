#include "input/hotkey_manager.hpp"
#include "SDL.h"


bool HotkeyManager::SetOpenWheelHotkey(Hotkey hotkey) {
    //Check if it conflicts with other hotkeys (currently no other hotkeys)
    openWheelHotkey = hotkey;
    return true;
}