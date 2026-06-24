#pragma once
#include "SDL.h"

namespace HotkeyTranslator {
    /**
     * @brief parses the SDL scancode into a keycode
     * @return the corresponding keycode (uiohook)
     */
    int ParseSDLScancode(const SDL_Scancode& scanCode);
    
    /**
     * @brief checks if the SDL_Scancode is invalid
     * @return if the scancode is invalid
     */
    bool IsScancodeInvalid(const SDL_Scancode& scanCode);
    
    inline const std::unordered_map<SDL_Keymod, Modifiers> HOTKEY_MODIFIERS = {
        {KMOD_SHIFT, Modifiers::Shift},
        {KMOD_CTRL, Modifiers::Ctrl},
        {KMOD_ALT, Modifiers::Alt}
    };
};