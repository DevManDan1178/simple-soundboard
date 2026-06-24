#include "input/hotkey_manager.hpp"
#include "SDL.h"
#include "input/hotkey_translator.hpp"
#include "input/hotkeys.hpp"

Modifiers& HotkeyManager::getModifierState() {
    Modifiers modifiers;
    SDL_Keymod sdlModifiers = SDL_GetModState();
    
    for (const auto& [sdlKeymod, modifierValue] : HotkeyTranslator::HOTKEY_MODIFIERS) {
        if (sdlModifiers & sdlKeymod) {
            modifiers |= modifierValue;
        }
    }
    return modifiers;   
}

Hotkey HotkeyManager::queryHotkey() {
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Scancode scanCode = event.key.keysym.scancode;
                
                if (HotkeyTranslator::IsScancodeInvalid(scanCode)) {
                    continue;
                }
                
                //Modifiers
                for (SDL_Scancode modifierCode : Hotkey::SDL_MODIFIER_CODES) {
                    if (modifierCode == scanCode) {
                        continue;
                    }
                }

                int keyCode = HotkeyTranslator::ParseSDLScancode(scanCode);
                
                Modifiers& modifiers = getModifierState();                
                return {keyCode, BindType::Keyboard, modifiers};
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT) {
                    continue;
                }

                Modifiers& modifiers = getModifierState();  
                return {(int) event.button.button, BindType::Mouse, modifiers};
            }

            if (event.type == SDL_QUIT) {
                return {};
            }
        }
    }
}