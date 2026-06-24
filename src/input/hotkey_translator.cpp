#include "input/hotkey_translator.hpp"
#include "uiohook.h"

int HotkeyTranslator::ParseSDLScancode(const SDL_Scancode& scanCode) {
    switch (scanCode)
    {
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


bool HotkeyTranslator::IsScancodeInvalid(const SDL_Scancode& scanCode) {
    return scanCode == VC_UNDEFINED;
}