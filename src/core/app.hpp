#pragma once
#include "soundboard/soundboard.hpp"
#include "config/config_manager.hpp"

class App {
public:
    Soundboard soundboard;
    ConfigManager configManager;

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
     * @brief The UI loop called ever frame
     */
    void UI();
    bool hiddenUI = false;
};