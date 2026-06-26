#include <iostream>
#include <vector>
#include <optional>
#include "audio/audio.hpp"
#include "soundboard/audio_table.hpp"

int AudioTable::CreateSubTable() {
    std::vector<Audio> newSubTable;
    data.push_back(newSubTable);
    return size();
}

void AudioTable::Clear() {
    data.clear();
}

bool AudioTable::AddAudio(Audio& audio, size_t tableIndex) {
    if (tableIndex >= size()) {
        std::cout << "Attempted insert into invalid index of AudioTable (" << tableIndex << ")" << std::endl;
        return false;
    }
    std::vector<Audio>& subTable = data[tableIndex];
    if (subTable.size() >= MAX_SUBTABLE_SIZE) {
        std::cout << "Attempted insert into already full index of AudioTable (" << tableIndex << ")" << std::endl;
        return false;
    }
    subTable.push_back(audio);
    std::cout << "Successful insert into AudioTable at index: (" << tableIndex << ")" << std::endl;
    return true;
}


bool AudioTable::RemoveAudio(size_t tableIndex, size_t index) {
    if (tableIndex >= size()) {
        std::cout << "Attempted removal from invalid index of AudioTable (" << tableIndex << ")" << std::endl;
        return false;
    }
    std::vector<Audio>& subTable = data[tableIndex];
    if (index >= subTable.size()) {
        std::cout << "Attempted removal from invalid subindex of AudioTable (" << tableIndex << ", " << index << ")" << std::endl;
        return false;
    }
    subTable.erase(subTable.begin() + index);
    std::cout << "Successful removal from AudioTable at table index (" << tableIndex << ") and index (" << index << ")" << std::endl;
    return true;
}


std::vector<Audio>* AudioTable::GetAudiosAtIdx(size_t tableIndex) {
    if (tableIndex >= size()) {
        std::cout << "Attempted query of invalid index of AudioTable (" << tableIndex << ")" << std::endl;
        return nullptr;
    }
    return &data[tableIndex];
}


std::optional<Audio> AudioTable::GetAudio(size_t tableIndex, size_t index) {
    std::vector<Audio>& subTable = *GetAudiosAtIdx(tableIndex);
    std::optional<Audio> audio;
    if (index >= subTable.size()) {
        return audio;
    }
    return subTable[index];
}