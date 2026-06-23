#pragma once
#include <filesystem>
#include <string>

struct Audio {
    std::filesystem::path filePath;
    float volume;
    std::string name;

    Audio(std::filesystem::path path, float vol = 1.0f) : filePath(path), volume(vol) {
        name = path.string();
    }
    
    Audio(std::filesystem::path path, float vol, std::string audioName) : filePath(path), volume(vol), name(audioName) {}

    bool operator==(const Audio& other) const {
        return other.name == name  && other.filePath == filePath && other.volume == volume;
    }
};

