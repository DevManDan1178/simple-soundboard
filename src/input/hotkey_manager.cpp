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

const std::function onStopAllAudioHotkeyPressed = []() -> void {
    EventDispatcher::Emit(StopAllSoundsEvent());
};

HotkeyManager::HotkeyManager() {
    for (const bool& scrollDir : {false, true}) {
        InputListener::SetOnScroll(scrollDir, [&scrollDir]() -> void {
            EventDispatcher::Emit(WheelScrollEvent(scrollDir));
        });
    }
}

bool HotkeyManager::SetStopAllAudioHotkey(Hotkey hotkey) {
    if (hotkey == stopAllAudioHotkey) {
        return true;
    } else if (hotkey == openWheelHotkey) {
        return false;
    }

    Hotkey previousHotkey = stopAllAudioHotkey;   
    stopAllAudioHotkey = hotkey;
    
    InputListener::SetOnHotkeyPress(hotkey, onStopAllAudioHotkeyPressed);
    
    InputListener::RemoveOnHotkeyPress(previousHotkey);

    EventDispatcher::Emit(ConfigChangeEvent());
    return true;
}

bool HotkeyManager::SetOpenWheelHotkey(Hotkey hotkey) {
    if (hotkey == openWheelHotkey) {
        return true;
    } else if (hotkey == stopAllAudioHotkey) {
        return false;
    }
    Hotkey previousHotkey = openWheelHotkey;   
    openWheelHotkey = hotkey;
    
    InputListener::SetOnHotkeyPress(hotkey, onOpenWheelHotkeyPressed);
    InputListener::SetOnHotkeyRelease(hotkey, onOpenWheelHotkeyReleased);
    
    InputListener::RemoveOnHotkeyPress(previousHotkey);
    InputListener::RemoveOnHotkeyRelease(previousHotkey);
    
    EventDispatcher::Emit(ConfigChangeEvent());
    return true;
}