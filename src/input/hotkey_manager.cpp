#include "input/hotkey_manager.hpp"
#include "SDL.h"
#include "event/event_dispatcher.hpp"
#include <functional>
#include <iostream>

const std::function onOpenWheelHotkeyPressed = []() -> void {
    EventDispatcher::Emit(ToggleWheelEvent(true));
};

const std::function onOpenWheelHotkeyReleased = []() -> void {
    EventDispatcher::Emit(ToggleWheelEvent(false));
};


HotkeyManager::HotkeyManager() {
    for (const bool& scrollDir : {false, true}) {
        InputListener::SetOnScroll(scrollDir, [&scrollDir]() -> void {
            EventDispatcher::Emit(WheelScrollEvent(scrollDir));
        });
    }
}

bool HotkeyManager::SetOpenWheelHotkey(Hotkey hotkey) {
    //Check if it conflicts with other hotkeys (currently no other hotkeys)
    Hotkey& previousHotkey = openWheelHotkey;   
    openWheelHotkey = hotkey;
    
    InputListener::SetOnHotkeyPress(hotkey, onOpenWheelHotkeyPressed);
    InputListener::SetOnHotkeyRelease(previousHotkey, onOpenWheelHotkeyReleased);

    EventDispatcher::Emit(ConfigChangeEvent());
    return true;
}