#pragma once
#include "soundboard/soundboard.hpp"
#include "config/config_manager.hpp"
#include "event/event.hpp"

class App {
public:
    Soundboard soundboard;
    ConfigManager configManager;

    bool wheelVisible = false;
    unsigned int currentWheelIdx = 0;
    /**
     * @brief Initializes the app (keeps running until application end)
     */
    void Initialize();

private:
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
    bool hiddenUI = false;
};