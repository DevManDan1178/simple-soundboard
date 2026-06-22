#pragma once
#include <vector>
#include <string>
#include "imgui.h"

namespace WheelUI {
    int RenderSelectionWheel(const std::vector<std::string>& emotes, bool& open, ImVec2 center, float radius = 120.0f); 
};