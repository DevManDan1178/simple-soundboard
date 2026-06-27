#include "imgui.h"
#include <vector>
#include <windows.h>
#include <string>
#include <cmath>
#include "ui/wheel_UI.hpp"

constexpr float HALF = 0.5f;
constexpr float PI = 3.14159265358979f;
constexpr float TAU = PI * 2.0f;
constexpr float HALF_PI = PI * HALF;

constexpr float OUTER_WHEEL_PADDING = 40.0f;
constexpr float OUTER_WHEEL_BORDER_THICKNESS = 2.0f;

constexpr int CIRCLE_SEGMENTS = 64;
constexpr int ARC_SEGMENTS_PER_SLICE = 32;

constexpr float EMOTE_WHEEL_INNER_RADIUS_RATIO = 0.25f;
constexpr float EMOTE_WHEEL_OUTER_RADIUS_RATIO = 1.0f * 1.25f; //1.25f adjusts it to be the size of the outer circle
constexpr float EMOTE_WHEEL_GAP = 0.01f;
constexpr float CENTER_BUTTON_RADIUS = 65.0f;

constexpr ImU32 OUTER_WHEEL_FILL_COLOR = IM_COL32(20, 20, 20, 220);
constexpr ImU32 OUTER_WHEEL_BORDER_COLOR = IM_COL32(100, 100, 100, 200);
constexpr ImU32 EMOTE_SELECTED_COLOR = IM_COL32(80, 160, 255, 255);
constexpr ImU32 EMOTE_DEFAULT_COLOR = IM_COL32(50, 50, 50, 240);
constexpr ImU32 CENTER_BUTTON_COLOR = IM_COL32(35, 35, 35, 255);
constexpr ImU32 TEXT_COLOR = IM_COL32_WHITE;

constexpr unsigned int ELEMENT_NAME_MAX_LENGTH = 10;

std::string shortenString(const std::string& str, int preEllipsisLength) {
    if (str.length() <= preEllipsisLength) {
        return str;
    }

    return str.substr(0, preEllipsisLength) + "...";
}

//↓↓ Full credit to [chatgpt.com] for this one ↓↓ 

int WheelUI::RenderSelectionWheel(
    const std::vector<std::string>& elements,
    const int& wheelIndex,
    bool& openParameter,
    float radius
){
    int hovered = -1;
    if (!openParameter) {
        return -1;
    }


    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    size_t count = elements.size();

    if (count == 0) {
        return -1;
    }

    float innerRadius = radius * EMOTE_WHEEL_INNER_RADIUS_RATIO;
    float outerRadius = radius;
    float angleStep = (TAU) / count;

    ImVec2 center = ImVec2(
        ImGui::GetIO().DisplaySize.x * 0.5f,
        ImGui::GetIO().DisplaySize.y * 0.5f
    );

    /* Background circle
    draw->AddCircleFilled(
        center,
        radius + OUTER_WHEEL_PADDING,
        OUTER_WHEEL_FILL_COLOR,
        CIRCLE_SEGMENTS
    );


    draw->AddCircle(
        center,
        radius + OUTER_WHEEL_PADDING,
        OUTER_WHEEL_BORDER_COLOR,
        CIRCLE_SEGMENTS,
        OUTER_WHEEL_BORDER_THICKNESS
    );
    */

    // Replace the mouse position block inside the per-slice loop.
// Move this ABOVE the loop (compute once, not per-slice):

    POINT cursorScreen{};
    GetCursorPos(&cursorScreen);
    // Convert screen coords to window-client coords so they match
    // the ImGui draw origin (which is client-space)
    ScreenToClient(GetActiveWindow(), /* or cache hwnd */ &cursorScreen);
    ImVec2 mousePos = ImVec2((float)cursorScreen.x, (float)cursorScreen.y);

    for (int i = 0; i < count; i++) {
        float startAngle = i * angleStep - HALF_PI + EMOTE_WHEEL_GAP;
        float endAngle = (i + 1) * angleStep - HALF_PI - EMOTE_WHEEL_GAP;
        
        std::vector<ImVec2> points;

        // Outer arc
        for (int j = 0; j <= ARC_SEGMENTS_PER_SLICE; j++) {
            float t = j / (float)ARC_SEGMENTS_PER_SLICE;
            float angle = startAngle + (endAngle - startAngle) * t;
            points.push_back(ImVec2(center.x + cosf(angle) * outerRadius, center.y + sinf(angle) * outerRadius));
        }



        // Inner arc
        for (int j = ARC_SEGMENTS_PER_SLICE; j >= 0; j--) {
            float t = j / (float) ARC_SEGMENTS_PER_SLICE;

            float angle = startAngle + (endAngle - startAngle) * t;


            points.push_back(ImVec2(center.x + cosf(angle) * innerRadius, center.y + sinf(angle) * innerRadius));
        }


        // Mouse offset from wheel center
        
        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;

        // Distance from center
        float distance = sqrtf(dx * dx + dy * dy);

        // Mouse angle
        float mouseAngle = atan2f(dy, dx);

        // Convert to [0, 2*PI]
        mouseAngle = mouseAngle < 0 ? mouseAngle + TAU : mouseAngle;



        float normalizedStart = startAngle;
        float normalizedEnd = endAngle;


        // Normalize slice angles
        while (normalizedStart < 0) {
            normalizedStart += TAU;
        } 

        while (normalizedEnd < 0) {
           normalizedEnd += TAU; 
        } 

        bool insideAngle;

        // Normal slice
        if (normalizedStart <= normalizedEnd) {
            insideAngle = mouseAngle >= normalizedStart && mouseAngle <= normalizedEnd;
        }
        // Slice crosses 0 degrees
        else {
            insideAngle = mouseAngle >= normalizedStart || mouseAngle <= normalizedEnd;
        }

        bool isHovered = distance >= innerRadius && insideAngle;

        if (isHovered) {
            hovered = i;
        }
            
        draw->AddConvexPolyFilled(
            points.data(),
            points.size(),
            isHovered ? EMOTE_SELECTED_COLOR : EMOTE_DEFAULT_COLOR
        );

        // Text
        float textAngle = (startAngle + endAngle) * HALF;
        float textRadius = (innerRadius + outerRadius) * HALF;
        std::string text = shortenString(elements[i], ELEMENT_NAME_MAX_LENGTH);
        ImVec2 textPos(center.x + cosf(textAngle) * textRadius, center.y + sinf(textAngle) * textRadius);
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

        draw->AddText(
            ImVec2(textPos.x - textSize.x * HALF, textPos.y - textSize.y * HALF ), 
            TEXT_COLOR, 
            text.c_str()
        );
    }

    // Center
    draw->AddCircleFilled(
        center,
        CENTER_BUTTON_RADIUS,
        CENTER_BUTTON_COLOR,
        CIRCLE_SEGMENTS
    );


    ImVec2 size = ImGui::CalcTextSize(std::to_string(wheelIndex).c_str());

    draw->AddText(
        ImVec2(center.x - size.x * HALF, center.y - size.y * HALF),
        TEXT_COLOR,
        std::to_string(wheelIndex).c_str()
    );

    if (hovered != -1 && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        openParameter = false;
    }

    return hovered;
}
