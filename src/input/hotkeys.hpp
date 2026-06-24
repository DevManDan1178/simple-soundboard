#pragma once

enum class BindType {
    KEYBOARD, MOUSE, NONE
};

enum class Modifiers : unsigned int {
    NONE = 0,
    SHIFT = 1u << 0,
    CTRL = 1u << 1,
    ALT = 1u << 2,
};  

constexpr Modifiers operator|(Modifiers l, Modifiers r) {
    return static_cast<Modifiers>(static_cast<unsigned int>(l) | static_cast<unsigned int>(r));
}

constexpr Modifiers operator&(Modifiers l, Modifiers r) {
    return static_cast<Modifiers>(static_cast<unsigned int>(l) & static_cast<unsigned int>(r));
}

struct Hotkey {
    int keyCode;
    BindType bindType;
    Modifiers modifiers;
    
    Hotkey()
        : keyCode(0), bindType(BindType::NONE), modifiers() {}

    Hotkey(int code, BindType type, Modifiers mods) : keyCode(code), bindType(type), modifiers(mods) {}

};