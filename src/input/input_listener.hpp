#pragma once
#include "input/hotkeys.hpp"
#include "functional"

using Callback = std::function<void()>;

namespace InputListener {
    /**
     * @brief Start the input listener
     */
    void Start();
    /**
     * @brief Stop the input listener
     */
    void Stop(); 

    /**
     * @brief Links the callback to the hotkey, calling it on every hotkey press
     * @param hotkey the hotkey
     * @param callback the callback
     */
    void SetOnHotkeyPress(const Hotkey& hotkey, const Callback& callback);
    
    /**
     * @brief Links the callback to the hotkey, calling it on every hotkey press
     * @param hotkey the hotkey
     * @param callback the callback
     */
    void SetOnHotkeyRelease(const Hotkey& hotkey, const Callback& callback);
    
    /**
     * @brief Unlinks the callback to the hotkey calling it on every press
     * @param hotkey the hotkey
     */
    void RemoveOnHotkeyPress(const Hotkey& hotkey);

    /**
     * @brief Unlinks the callback to the hotkey calling it on every release
     * @param hotkey the hotkey
     */
    void RemoveOnHotkeyRelease(const Hotkey& hotkey);
};