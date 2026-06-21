#pragma once
#include <filesystem>

struct Audio {
    std::filesystem::path audioPath;
    float volume;

    Audio(std::filesystem::path path) : audioPath(path), volume(1.0f) {}
};

