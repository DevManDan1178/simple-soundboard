#include "soundboard/soundboard.hpp"
#include <optional>

bool Soundboard::PlaySound(int tableIndex, int index) {
    std::optional<Audio> audio = audioTable.GetAudio(tableIndex, index);
    if (audio.has_value()) {
        audioManager.PlayAudio(*audio);
        return true;
    }
    return false;
}