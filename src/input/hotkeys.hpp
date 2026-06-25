#pragma once
#include "SDL.h"
#include <functional>

enum class BindType : uint8_t{
    None = 0,
    Keyboard = 1, 
    Mouse = 2, 
};

enum class Modifiers : unsigned int {
    None = 0,
    Shift = 1u << 0,
    Ctrl = 1u << 1,
    Alt = 1u << 2,
};  

constexpr Modifiers operator|(Modifiers l, Modifiers r) {
    return static_cast<Modifiers>(static_cast<unsigned int>(l) | static_cast<unsigned int>(r));
}

constexpr Modifiers& operator|=(Modifiers& l, Modifiers r) {
    l = l | r;
    return l;
}

constexpr Modifiers operator&(Modifiers l, Modifiers r) {
    return static_cast<Modifiers>(static_cast<unsigned int>(l) & static_cast<unsigned int>(r));
}



struct Hotkey {
    int keyCode;
    BindType bindType;
    Modifiers modifiers;
    
    Hotkey()
        : keyCode(0), bindType(BindType::None), modifiers() {}

    Hotkey(int code, BindType type, Modifiers mods) : keyCode(code), bindType(type), modifiers(mods) {}

    constexpr static SDL_Scancode SDL_MODIFIER_CODES[6] = {
        SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT,
        SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL,
        SDL_SCANCODE_LALT, SDL_SCANCODE_RALT
    };
    bool operator==(const Hotkey&) const = default;
};

namespace std {
template<>
    struct std::hash<Hotkey> {
        size_t operator()(const Hotkey& h) const {
            return h.keyCode ^
                (static_cast<unsigned int>(h.modifiers) << 8) ^
                (static_cast<uint8_t>(h.bindType) << 9);
        }
    };
};
