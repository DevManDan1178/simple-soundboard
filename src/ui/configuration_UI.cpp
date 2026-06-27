#include "imgui.h"
#include "ui/configuration_UI.hpp"
#include "ui/file_dialog.hpp"
#include "input/input.hpp"
#include "event/event_dispatcher.hpp"
#include <format>
#include <string>
#include <filesystem>

constexpr float INDENT = 10.0f;

constexpr float VOLUME_SLIDER_WIDTH = 200.0f;
constexpr float MASTER_VOLUME_SLIDER_WIDTH = 300.0f;
constexpr float WHEEL_DISPLAY_PADDING_TOP = 5.0f;
constexpr float AUDIO_DISPLAY_PADDING_X = 20.0f;

constexpr float ADD_AUDIO_PADDING_TOP = 10.0f; //Only if there are audio displays above
constexpr float ADD_WHEEL_PADDING_TOP = 15.0f; //Only if there are wheel displays above

constexpr float HOTKEY_BUTTON_MINIMUM_WIDTH = 75.0f;
constexpr float HOTKEY_BUTTON_PADDING_TOP = 5.0f;

constexpr float SECTION_TITLE_PADDING_BOTTOM = 5.0f;
constexpr float SECTION_PADDING = 20.0f;

constexpr const char* HOTKEY_CONFIGURATIONS_TITLE = "------ HOTKEYS ------";
constexpr const char* AUDIO_CONFIGURATIONS_TITLE  = "--- SOUND EFFECTS ---";
constexpr const char* DRIVER_CONFIGURATIONS_TITLE = "--- AUDIO DRIVERS ---";

void AUDIO_DISPLAY_PADDING() {
    ImGui::SameLine(0, AUDIO_DISPLAY_PADDING_X);
}



void ConfigurationUI::RenderDriverConfigurations(AudioManager& audioManager) {
    ImGui::Text(DRIVER_CONFIGURATIONS_TITLE); 
    ImGui::Dummy(ImVec2(0, SECTION_TITLE_PADDING_BOTTOM));
    ImGui::Indent(INDENT);
    static std::vector<std::string> audioDrivers = audioManager.GetAvailableDrivers();
    static int selectedDriver = 0;
    if (audioDrivers.empty()) {
        ImGui::Text("[!] No Audio Drivers Detected");
        ImGui::SameLine();
        if (ImGui::Button("Retry")) {
            audioDrivers = audioManager.GetAvailableDrivers();
        }
        return;
    }

    if (ImGui::BeginCombo("Audio Driver", audioDrivers[selectedDriver].c_str())) {
        audioDrivers = audioManager.GetAvailableDrivers();
        if (audioDrivers.empty())
        {
            ImGui::TextDisabled("No audio drivers detected");
            ImGui::EndCombo();
            return;
        }
        
        for (int i = 0; i < audioDrivers.size(); ++i) {
            bool isSelected = (selectedDriver == i);

            if (ImGui::Selectable(audioDrivers[i].c_str(), isSelected))
            {
                selectedDriver = i;

                // Switch to the selected driver
                audioManager.SetAudioDriver(i);
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }
     ImGui::Unindent(INDENT);
}

void ConfigurationUI::RenderHotkeyConfigurations(HotkeyManager& hotkeyManager) {
    Hotkey openWheelHotkey = hotkeyManager.openWheelHotkey;
    
    ImGui::Text(HOTKEY_CONFIGURATIONS_TITLE); 
    ImGui::Dummy(ImVec2(0, SECTION_TITLE_PADDING_BOTTOM));

    ImGui::Indent(INDENT);
    ImGui::Text("Open Emote Wheel");
    
    
    static auto hotkeyButton = [](const char* buttonText) -> bool {
        ImGui::Dummy(ImVec2(0, HOTKEY_BUTTON_PADDING_TOP));
        float textWidth = ImGui::CalcTextSize(buttonText).x;
        float width = (textWidth + ImGui::GetStyle().FramePadding.x * 2.0f);
        width = (width < HOTKEY_BUTTON_MINIMUM_WIDTH) ? HOTKEY_BUTTON_MINIMUM_WIDTH : width;
        return ImGui::Button(buttonText, ImVec2(width, 0.0f));
    };

    ImGui::Indent(INDENT);
    
    if (hotkeyButton(std::format("{0}##OpenWheel", Input::HotkeyToString(openWheelHotkey)).c_str())) {
        hotkeyManager.SetOpenWheelHotkey(Input::QueryHotkey());
    }
    ImGui::Unindent(INDENT);

    ImGui::Dummy(ImVec2(0, SECTION_TITLE_PADDING_BOTTOM));
    ImGui::Text("[!] Modifier keys: [Ctrl], [Alt], [Shift]");
    
    ImGui::Unindent(INDENT);
}

void ConfigurationUI::RenderAudioConfigurations(Soundboard& soundboard) {
    static bool unsavedChanges = false;
    AudioTable& audioTable = soundboard.audioTable;
    AudioManager& audioManager = soundboard.audioManager;

    ImGui::Text(AUDIO_CONFIGURATIONS_TITLE);
    ImGui::Dummy(ImVec2(0, SECTION_TITLE_PADDING_BOTTOM));
    ImGui::Indent(INDENT);

    float* masterVolume = &audioManager.masterVolume;

    ImGui::SetNextItemWidth(MASTER_VOLUME_SLIDER_WIDTH);
    if (ImGui::SliderFloat("##MasterVolume", masterVolume, 0, 2, "Master Volume: %.2f")) {
        unsavedChanges = true;
    }
    ImGui::SameLine();
    ImGui::Text("[Ctrl + Click] for Manual Input");

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
            if (ImGui::SliderFloat(std::format("##{0}-{1}", tableIdx, i).c_str(), &audio.volume, 0, 2, "Volume: %.2f")) {
                unsavedChanges = true;
            }
            
            AUDIO_DISPLAY_PADDING();

            if (ImGui::Button(std::format("Remove Audio##{0}-{1}", tableIdx, i).c_str())) {
                audioTable.RemoveAudio(tableIdx, i);
                unsavedChanges = true;
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
                    unsavedChanges = true;
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
        unsavedChanges = true;
    }

    ImGui::Dummy(ImVec2(0, wheelsCount > 0 ? ADD_WHEEL_PADDING_TOP : WHEEL_DISPLAY_PADDING_TOP));
    
    if (ImGui::Button(unsavedChanges ? "[!] Save" : "Saved")) {
        EventDispatcher::Emit(ConfigChangeEvent());
        unsavedChanges = false;
    }

    ImGui::Unindent(INDENT);
}

void ConfigurationUI::Render(Soundboard& soundboard) {
    ImGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true); //Prevents tab from focusing elements

    RenderAudioConfigurations(soundboard);
    ImGui::Dummy(ImVec2(0, SECTION_PADDING));
    RenderHotkeyConfigurations(soundboard.hotkeyManager);
    ImGui::Dummy(ImVec2(0, SECTION_PADDING));
    RenderDriverConfigurations(soundboard.audioManager);
    ImGui::PopItemFlag();
}