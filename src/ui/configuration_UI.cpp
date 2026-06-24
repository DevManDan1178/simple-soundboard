#include "imgui.h"
#include "ui/configuration_UI.hpp"
#include "ui/file_dialog.hpp"
#include <format>
#include <string>
#include <filesystem>

constexpr float INDENT = 20.0f;
constexpr float VOLUME_SLIDER_WIDTH = 200.0f;
constexpr float MASTER_VOLUME_SLIDER_WIDTH = 400.0f;
constexpr float WHEEL_DISPLAY_PADDING_TOP = 5.0f;
constexpr float AUDIO_DISPLAY_PADDING_X = 20.0f;
constexpr float ADD_AUDIO_PADDING_TOP = 10.0f; //Only if there are audio displays above
constexpr float ADD_WHEEL_PADDING_TOP = 15.0f; //Only if there are wheel displays above

void AUDIO_DISPLAY_PADDING() {
    ImGui::SameLine(0, AUDIO_DISPLAY_PADDING_X);
}

void ConfigurationUI::RenderAudioConfigurations(Soundboard& soundboard) {
    AudioTable& audioTable = soundboard.audioTable;
    AudioManager& audioManager = soundboard.audioManager;
    ImGui::Indent(INDENT);

    float* masterVolume = &audioManager.masterVolume;

    ImGui::SetNextItemWidth(MASTER_VOLUME_SLIDER_WIDTH);
    ImGui::SliderFloat("##MasterVolume", masterVolume, 0, 2, "Master Volume: %.2f");

    const size_t wheelsCount = audioTable.size();
    for (size_t tableIdx = 0; tableIdx < wheelsCount; tableIdx++) {  
        ImGui::Dummy(ImVec2(0, WHEEL_DISPLAY_PADDING_TOP));
        
        std::vector<Audio>& audios = *audioTable.GetAudiosAtIdx(tableIdx);

        ImGui::Text(std::format("Wheel {0}", tableIdx + 1).c_str());   
        ImGui::Indent(INDENT);
        
        const size_t audioCount = audios.size();
        for (size_t i = 0; i < audioCount; i++) {
            Audio& audio = audios[i];
            
            if (ImGui::Button(std::format("{0}##{1}-{2}", audio.name, tableIdx, i).c_str())) {
                soundboard.PlayAudio(tableIdx, i);
            }

            AUDIO_DISPLAY_PADDING();

            ImGui::SetNextItemWidth(VOLUME_SLIDER_WIDTH);
            ImGui::SliderFloat(std::format("##{0}-{1}", tableIdx, i).c_str(), &audio.volume, 0, 2, "Volume: %.2f");
            
            AUDIO_DISPLAY_PADDING();

            if (ImGui::Button(std::format("Remove Audio##{0}-{1}", tableIdx, i).c_str())) {
                audioTable.RemoveAudio(tableIdx, i);
            }
        }

        if (audioCount > 0) {
            ImGui::Dummy(ImVec2(0, ADD_AUDIO_PADDING_TOP));
        }

        if (audioCount < AudioTable::MAX_SUBTABLE_SIZE) {
            
            if (ImGui::Button(std::format("Add a sound##{0}", tableIdx).c_str())) {            
                std::filesystem::path queryPath = FileDialog::OpenAudioFile();
                
                if (!queryPath.empty()) {
                    Audio newAudio = Audio(queryPath);
                    bool success = audioTable.AddAudio(newAudio, tableIdx);
                    if (success) {
                        soundboard.PlayAudio(tableIdx, audios.size() - 1);
                    }
                }
            }
        } else {
            ImGui::Text("Maximum Reached");
        }

        ImGui::Unindent(INDENT);
    }

    ImGui::Dummy(ImVec2(0, wheelsCount > 0 ? ADD_WHEEL_PADDING_TOP : WHEEL_DISPLAY_PADDING_TOP));
    
    if (ImGui::Button("Add a Wheel")) {
        int newWheelIdx = audioTable.CreateSubTable();
    }

    ImGui::Unindent(INDENT);
}

void ConfigurationUI::RenderKeybindConfigurations(Soundboard& soundboard) {
    //Changing keybinds
}

void ConfigurationUI::Render(Soundboard& soundboard) {
    RenderAudioConfigurations(soundboard);
}