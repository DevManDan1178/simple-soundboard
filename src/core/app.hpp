#pragma once
#include "soundboard/soundboard.hpp"
#include "config/config_manager.hpp"
#include "event/event.hpp"

#include "SDL.h"
#include <SDL_opengl.h>
#include <SDL_syswm.h>

class App {
public:
    Soundboard soundboard;
    ConfigManager* configManager;
    bool wheelVisible = false;
    unsigned int wheelIndex = 0;
    int wheelSelectedIndex = -1;

    App();

    /**
     * @brief Initializes the app (keeps running until application end)
     */
    void Initialize();

    
private:
    SDL_Window* window;
    HWND hwnd;
    int configPositionX;
    int configPositionY;
    int configWidth;
    int configHeight;
    bool unsavedAudioChanges;
    constexpr static const int WHEEL_UI_SIZE = 400;
    /**
     * @brief Initializes the UI loop (keeps running until application end)
     */
    void InitializeUI();

    /**
     * @brief Handles events from the event dispatcher (input detection)
     * @param Event event object
     */
    void HandleEvent(const Event& event);
    
    /**
     * @brief Shows the wheel UI
     * @param elements elements to show in the wheel
     */
    void ShowWheel(std::vector<std::string> elements);
    
    /**
     * @brief The UI loop called ever frame
     */
    void UI();

    void ToggleWheelVisible(bool visible);
    
};