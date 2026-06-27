#include "input/input_listener.hpp"
#include "uiohook.h"
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_map>
#include <Windows.h>


using HotkeyCallbacks = std::unordered_map<Hotkey, Callback>;

/**
 * Callbacks for scrolling
 */
std::unordered_map<bool, Callback> scroll_bindings;

/**
 * Hotkey callbacks for pressed
 */
HotkeyCallbacks pressed_bindings;

/**
 * Hotkey callbacks for released
 */
HotkeyCallbacks released_bindings;

/**
 * Maps keycodes to their pressed state (mouse keycodes are negative)
 */
std::unordered_map<int, bool> keystates;

constexpr int MODIFIER_KEYCODES[6] = {
    VC_CONTROL_L, VC_CONTROL_R, 
    VC_SHIFT_L, VC_SHIFT_R, 
    VC_ALT_L, VC_ALT_R,
};

const std::unordered_map<uint16_t, Modifiers> MASK_MODIFIERS = {
    {MASK_SHIFT, Modifiers::Shift},
    {MASK_ALT, Modifiers::Alt},
    {MASK_CTRL, Modifiers::Ctrl},
};

bool IsKeyModifier(int keycode) {
    for (int modifierKeycodes : MODIFIER_KEYCODES) {
        if (keycode == modifierKeycodes) {
            return true;
        }
    }
    return false;
}

Modifiers GetUIOHOOKEventModifiers(uint16_t eventMask) {
    Modifiers mods = Modifiers::None;
    for (auto& [mask, modifier] : MASK_MODIFIERS) {
        if (eventMask & mask) {
            mods |= modifier;
        }
    }
    return mods;
}

void DispatchUIOHOOKEvent(uiohook_event* event) {
    int code;
    bool isKeyboard, isPressed;
    switch(event->type) {
        case EVENT_KEY_PRESSED:
            isKeyboard = isPressed = true;
            break;
        case EVENT_KEY_RELEASED:
            isKeyboard = !(isPressed = false);
            break;
        case EVENT_MOUSE_PRESSED:
            isKeyboard = !(isPressed = true);
            break;
        case EVENT_MOUSE_RELEASED:
            isKeyboard = isPressed = false;
            break;
        case EVENT_MOUSE_WHEEL: {
            bool scrollDir = event-> data.wheel.direction < 0;
            
            if (!scroll_bindings.contains(scrollDir)) {
                return;
            }

            scroll_bindings[scrollDir]();        
            return;
        }
        default:
            return;
    }
    code = isKeyboard ? event->data.keyboard.keycode : event->data.mouse.button;
    bool& keyPressedState = keystates[isKeyboard ? code : -code];
    
    //Avoid repeat signals from holding
    if (isPressed && keyPressedState) {
        return;
    }

    keyPressedState = isPressed;

    //Ignore modifiers
    if (isKeyboard && IsKeyModifier(code)) {
        return;
    }

    Hotkey detectedHotkey{
        code, 
        isKeyboard ? BindType::Keyboard : BindType::Mouse, 
        isPressed ? GetUIOHOOKEventModifiers(event->mask) : Modifiers::None //No modifiers on key release for consistency
    };
    
    HotkeyCallbacks& hotkeyCallbackList = isPressed ? pressed_bindings : released_bindings;
    if (!hotkeyCallbackList.contains(detectedHotkey)) {
        return;
    }
    hotkeyCallbackList[detectedHotkey]();
}

void InputListener::SetOnScroll(bool scrollDirection, const Callback& callback) {
    scroll_bindings[scrollDirection] = callback;
}

void InputListener::SetOnHotkeyPress(const Hotkey& hotkey, const Callback& callback) {
    pressed_bindings[hotkey] = callback;
}

void InputListener::SetOnHotkeyRelease(const Hotkey& hotkey, const Callback& callback) {
    Hotkey modlessHotkey = Hotkey{hotkey.keyCode, hotkey.bindType, Modifiers::None};
    released_bindings[modlessHotkey] = callback;
}

void InputListener::RemoveOnHotkeyPress(const Hotkey& hotkey) {
    pressed_bindings.erase(hotkey);
}


void InputListener::RemoveOnHotkeyRelease(const Hotkey& hotkey) {
    released_bindings.erase(hotkey);
}

void InputListener::Start() {
    hook_set_dispatch_proc(DispatchUIOHOOKEvent);
    
    std::thread([](){
        int result = hook_run();
        if (result != UIOHOOK_SUCCESS) {
            std::cerr << "hook_run() failed in [Input Listener] Start()" << std::endl;
        }
    }).detach();
}


void InputListener::Stop() {
    hook_stop();
}
