#pragma once
#include <filesystem>
#include <string>
#include <string_view>

struct Audio {
    std::filesystem::path filePath;
    float volume;
    std::string name;

    Audio(const std::filesystem::path& path, const float vol = 1.0f) : filePath(path), volume(vol) {
        std::string fullPathStr = path.string();
        std::string::size_type filenameStartIdx = fullPathStr.rfind('\\');

        std::string filename = (filenameStartIdx == std::string::npos) ? 
            std::string{fullPathStr} : 
            std::string{fullPathStr}.substr(filenameStartIdx + 1);
        
        std::string::size_type filetypeStartIdx = filename.rfind('.');
        
        name = (filetypeStartIdx == std::string::npos) ? 
            std::string{filename} : 
            std::string{filename}.substr(0, filetypeStartIdx);
    }
    
    Audio(const std::filesystem::path& path, float vol, std::string audioName) : filePath(path), volume(vol), name(audioName) {}

    bool operator==(const Audio& other) const {
        return other.name == name  && other.filePath == filePath && other.volume == volume;
    }
};

