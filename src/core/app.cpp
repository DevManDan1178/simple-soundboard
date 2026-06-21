#include <filesystem>
#include <iostream>
#include "core/app.hpp"
#include "ui/file_dialog.hpp"
#include "audio/audio_manager.hpp"
#include "audio/audio.hpp"

void App::Initialize() {
    AudioManager audioManager;
    
    std::filesystem::path filePath = FileDialog::OpenAudioFile();
    Audio audio(filePath);
    std::cout << "Attempting to play audio [" << audio.audioPath << "] at volume [" << audio.volume << "]" << std::endl;
    audioManager.LoadAudio(audio);
    audioManager.PlayAudio(audio);
    std::cin.get();
    return;
}