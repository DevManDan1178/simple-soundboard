#pragma once
#include <vector>
#include <string>
#include "audio/audio.hpp"
#include "audio/audio_manager.hpp"
#include "soundboard/audio_table.hpp"
#include "input/hotkey_manager.hpp"

class Soundboard {
public:
    AudioTable audioTable;
    AudioManager audioManager;
    HotkeyManager hotkeyManager;

    /**
     * @brief plays the sound in the sound tables
     * @param tableIndex index of the table [from 0]
     * @param index index in the table [from 0]
     * @return if it was successful or not
     */
    bool PlayAudio(int tableIndex, int index);
};