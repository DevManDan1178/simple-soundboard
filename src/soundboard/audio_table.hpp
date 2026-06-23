#pragma once
#include <vector>
#include "audio/audio.hpp"
#include <optional>

/**
 * @brief table for a soundboard emote wheel: 2D vector (first the wheel's index, then the index in the wheel)
 */
class AudioTable {
private:
    std::vector<std::vector<Audio>> data;

public:
    constexpr static const int MAX_SUBTABLE_SIZE = 6;
    
    /**
     * @brief adds an audio to the table index if possible
     * @param tableIndex index of the table [from 0]
     * @return if it was successful or not
     */
    bool AddAudio(Audio& audio, size_t tableIndex);

    /**
     * @brief appends an empty subtable to the table
     * @return index of the new subtable [from 0]
     */
    int CreateSubTable();

    /**
     * @brief removes the audio at the corresponding position
     * @param tableIndex index of the table [from 0]
     * @param index index in the table [from 0]
     * @return if it was successful or not
     */
    bool RemoveAudio(size_t tableIndex, size_t index);
    
    /**
     * @brief gets the list of audios at the table index
     * @param tableIndex index of the table [from 0]
     * @return list of audios at the table index (pointer)
     */
    std::vector<Audio>* GetAudiosAtIdx(size_t tableIndex);

    /**
     * @brief gets the audio at the corresponding position
     * @param tableIndex index of the table [from 0]
     * @param index index in the table [from 0]
     * @return the audio (optional, copy)
     */
    std::optional<Audio> GetAudio(size_t tableIndex, size_t index);

    /**
     * @brief gets size of table (amount of audio lists)
    */
    size_t size(){
        return data.size();
    }
};