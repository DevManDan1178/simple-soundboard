#pragma once
#include <vector>
#include <string>
#include "imgui.h"

namespace WheelUI {
    /**
     * Renders the selection wheel and returns the index of the element being hovered (-1 if none)
     * @param elements elements on the wheel
     * @param openParameter reference to the boolean that controls if it is open
     * @param center center position of the wheel
     * @param radius radius of the wheel (default = 120.0f)
     */
    int RenderSelectionWheel(const std::vector<std::string>& elements, bool& openParameter, ImVec2 center, float radius = 120.0f); 
};