#include "input/input.hpp"
#include "uiohook.h"
#include <string>
#include <sstream>

Modifiers Input::GetModifierState() {
    Modifiers modifiers = Modifiers::None;
    SDL_Keymod sdlModifiers = SDL_GetModState();
    
    for (const auto& [sdlKeymod, modifierValue] : HOTKEY_MODIFIERS) {
        if (sdlModifiers & sdlKeymod) {
            modifiers |= modifierValue;
        }
    }
    return modifiers;
}

Hotkey Input::QueryHotkey() {
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Scancode scanCode = event.key.keysym.scancode;
                
                if (IsScancodeInvalid(scanCode)) {
                    continue;
                }
                
                //Modifiers
                bool isModifier = false;
                for (SDL_Scancode modifierCode : Hotkey::SDL_MODIFIER_CODES) {
                    if (modifierCode == scanCode) {
                        isModifier = true;
                        break;
                    }
                }
                if (isModifier) {
                    continue;
                }

                int keyCode = ParseSDLScancode(scanCode);
                
                Modifiers modifiers = GetModifierState();                
                return {keyCode, BindType::Keyboard, modifiers};
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT) {
                    continue;
                }

                Modifiers modifiers = GetModifierState();  
                return {(int) event.button.button, BindType::Mouse, modifiers};
            }

            if (event.type == SDL_QUIT) {
                return {};
            }
        }
    }
}


bool Input::IsScancodeInvalid(const SDL_Scancode& scanCode) {
    return scanCode == VC_UNDEFINED;
}


int Input::ParseSDLScancode(const SDL_Scancode& scanCode) {
    switch (scanCode) {
        case SDL_SCANCODE_A: return VC_A;
        case SDL_SCANCODE_B: return VC_B;
        case SDL_SCANCODE_C: return VC_C;
        case SDL_SCANCODE_D: return VC_D;
        case SDL_SCANCODE_E: return VC_E;
        case SDL_SCANCODE_F: return VC_F;
        case SDL_SCANCODE_G: return VC_G;
        case SDL_SCANCODE_H: return VC_H;
        case SDL_SCANCODE_I: return VC_I;
        case SDL_SCANCODE_J: return VC_J;
        case SDL_SCANCODE_K: return VC_K;
        case SDL_SCANCODE_L: return VC_L;
        case SDL_SCANCODE_M: return VC_M;
        case SDL_SCANCODE_N: return VC_N;
        case SDL_SCANCODE_O: return VC_O;
        case SDL_SCANCODE_P: return VC_P;
        case SDL_SCANCODE_Q: return VC_Q;
        case SDL_SCANCODE_R: return VC_R;
        case SDL_SCANCODE_S: return VC_S;
        case SDL_SCANCODE_T: return VC_T;
        case SDL_SCANCODE_U: return VC_U;
        case SDL_SCANCODE_V: return VC_V;
        case SDL_SCANCODE_W: return VC_W;
        case SDL_SCANCODE_X: return VC_X;
        case SDL_SCANCODE_Y: return VC_Y;
        case SDL_SCANCODE_Z: return VC_Z;

        case SDL_SCANCODE_0: return VC_0;
        case SDL_SCANCODE_1: return VC_1;
        case SDL_SCANCODE_2: return VC_2;
        case SDL_SCANCODE_3: return VC_3;
        case SDL_SCANCODE_4: return VC_4;
        case SDL_SCANCODE_5: return VC_5;
        case SDL_SCANCODE_6: return VC_6;
        case SDL_SCANCODE_7: return VC_7;
        case SDL_SCANCODE_8: return VC_8;
        case SDL_SCANCODE_9: return VC_9;

        case SDL_SCANCODE_TAB: return VC_TAB;
        case SDL_SCANCODE_RETURN: return VC_ENTER;
        case SDL_SCANCODE_ESCAPE: return VC_ESCAPE;
        case SDL_SCANCODE_SPACE: return VC_SPACE;

        case SDL_SCANCODE_F1: return VC_F1;
        case SDL_SCANCODE_F2: return VC_F2;
        case SDL_SCANCODE_F3: return VC_F3;
        case SDL_SCANCODE_F4: return VC_F4;
        case SDL_SCANCODE_F5: return VC_F5;
        case SDL_SCANCODE_F6: return VC_F6;
        case SDL_SCANCODE_F7: return VC_F7;
        case SDL_SCANCODE_F8: return VC_F8;
        case SDL_SCANCODE_F9: return VC_F9;
        case SDL_SCANCODE_F10: return VC_F10;
        case SDL_SCANCODE_F11: return VC_F11;
        case SDL_SCANCODE_F12: return VC_F12;

        case SDL_SCANCODE_SEMICOLON: return VC_SEMICOLON;
        case SDL_SCANCODE_APOSTROPHE: return VC_QUOTE;
        case SDL_SCANCODE_COMMA: return VC_COMMA;
        case SDL_SCANCODE_PERIOD: return VC_PERIOD;
        case SDL_SCANCODE_SLASH: return VC_SLASH;
        case SDL_SCANCODE_BACKSLASH: return VC_BACK_SLASH;
        case SDL_SCANCODE_LEFTBRACKET: return VC_OPEN_BRACKET;
        case SDL_SCANCODE_RIGHTBRACKET: return VC_CLOSE_BRACKET;
        case SDL_SCANCODE_GRAVE: return VC_BACKQUOTE;
        default:
            return VC_UNDEFINED;
    }
}



std::string Input::HotkeyToString(const Hotkey& hotkey)
{
    std::stringstream ss;

    // Modifiers
    for (const auto& [modifierValue, modifierStr] : MODIFIER_STRINGS) {
        if ((hotkey.modifiers & modifierValue) != Modifiers::None) {
            ss << modifierStr;
        }
    }

    if (hotkey.bindType == BindType::Keyboard) {
        switch (hotkey.keyCode) {
            case VC_A: ss << "A"; break;
            case VC_B: ss << "B"; break;
            case VC_C: ss << "C"; break;
            case VC_D: ss << "D"; break;
            case VC_E: ss << "E"; break;
            case VC_F: ss << "F"; break;
            case VC_G: ss << "G"; break;
            case VC_H: ss << "H"; break;
            case VC_I: ss << "I"; break;
            case VC_J: ss << "J"; break;
            case VC_K: ss << "K"; break;
            case VC_L: ss << "L"; break;
            case VC_M: ss << "M"; break;
            case VC_N: ss << "N"; break;
            case VC_O: ss << "O"; break;
            case VC_P: ss << "P"; break;
            case VC_Q: ss << "Q"; break;
            case VC_R: ss << "R"; break;
            case VC_S: ss << "S"; break;
            case VC_T: ss << "T"; break;
            case VC_U: ss << "U"; break;
            case VC_V: ss << "V"; break;
            case VC_W: ss << "W"; break;
            case VC_X: ss << "X"; break;
            case VC_Y: ss << "Y"; break;
            case VC_Z: ss << "Z"; break;

            case VC_0: ss << "0"; break;
            case VC_1: ss << "1"; break;
            case VC_2: ss << "2"; break;
            case VC_3: ss << "3"; break;
            case VC_4: ss << "4"; break;
            case VC_5: ss << "5"; break;
            case VC_6: ss << "6"; break;
            case VC_7: ss << "7"; break;
            case VC_8: ss << "8"; break;
            case VC_9: ss << "9"; break;

            case VC_TAB:        ss << "Tab"; break;
            case VC_ENTER:      ss << "Enter"; break;
            case VC_ESCAPE:     ss << "Escape"; break;
            case VC_SPACE:      ss << "Space"; break;
            case VC_BACKSPACE:   ss << "Backspace"; break;

            case VC_F1: ss << "F1"; break;
            case VC_F2: ss << "F2"; break;
            case VC_F3: ss << "F3"; break;
            case VC_F4: ss << "F4"; break;
            case VC_F5: ss << "F5"; break;
            case VC_F6: ss << "F6"; break;
            case VC_F7: ss << "F7"; break;
            case VC_F8: ss << "F8"; break;
            case VC_F9: ss << "F9"; break;
            case VC_F10: ss << "F10"; break;
            case VC_F11: ss << "F11"; break;
            case VC_F12: ss << "F12"; break;

            case VC_INSERT:     ss << "Insert"; break;
            case VC_DELETE:     ss << "Delete"; break;
            case VC_HOME:       ss << "Home"; break;
            case VC_END:        ss << "End"; break;
            case VC_PAGE_UP:    ss << "PageUp"; break;
            case VC_PAGE_DOWN:  ss << "PageDown"; break;

            case VC_UP:    ss << "Up"; break;
            case VC_DOWN:  ss << "Down"; break;
            case VC_LEFT:  ss << "Left"; break;
            case VC_RIGHT: ss << "Right"; break;

            case VC_SEMICOLON:     ss << ";"; break;
            case VC_QUOTE:         ss << "'"; break;
            case VC_BACK_SLASH:    ss << "\\"; break;
            case VC_OPEN_BRACKET:  ss << "["; break;
            case VC_CLOSE_BRACKET: ss << "]"; break;
            case VC_COMMA:         ss << ","; break;
            case VC_PERIOD:        ss << "."; break;
            case VC_SLASH:         ss << "/"; break;
            case VC_BACKQUOTE:     ss << "`"; break;
            default:
                ss << "Key" << hotkey.keyCode;
                break;
        }
    } else if (hotkey.bindType == BindType::Mouse) {
        switch (hotkey.keyCode)
        {
            case 1: ss << "Mouse Left"; break;
            case 2: ss << "Mouse Middle"; break;
            case 3: ss << "Mouse Right"; break;
            default: ss << "Mouse " << hotkey.keyCode; break;
        }
    } else {
        ss << "undefined";
    }

    return ss.str();
}