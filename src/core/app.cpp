#include <filesystem>
#include <iostream>
#include <functional>
#include <algorithm>

#include "core/app.hpp"
#include "ui/file_dialog.hpp"
#include "ui/configuration_UI.hpp"
#include "audio/audio_manager.hpp"
#include "audio/audio.hpp"
#include "event/event_dispatcher.hpp"
#include "config/config_manager.hpp"

#include "SDL.h"
#include <SDL_opengl.h>
#include <SDL_syswm.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "ui/wheel_UI.hpp"

#include <SDL.h>
#include <Windows.h>

void SetAlpha(SDL_Window* window, HWND hwnd, float alpha)
{
    // clamp alpha to valid range
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    BYTE winAlpha = static_cast<BYTE>(alpha * 255.0f);

    // Enable layered window if not already
    LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);

    // Apply alpha
    SetLayeredWindowAttributes(
        hwnd,
        0,
        winAlpha,
        LWA_ALPHA
    );
}
void SetBorderless(SDL_Window* window, HWND hwnd, bool borderless)
{
    SDL_SetWindowBordered(window, borderless ? SDL_FALSE : SDL_TRUE);

    SetWindowPos(
        hwnd,
        nullptr,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE
    );
}

void SetAlwaysOnTop(SDL_Window* window, HWND hwnd, bool alwaysOnTop) {
    (void)window; // unused, but kept for symmetry

    if (alwaysOnTop) {
        SetWindowPos(
            hwnd,
            HWND_TOPMOST,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
        );
    } else {
        SetWindowPos(
            hwnd,
            HWND_NOTOPMOST,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
        );
    }
}


void App::ToggleWheelVisible(bool visible)
{
    wheelVisible = visible;
    if (visible) {
        SetAlpha(window, hwnd, 0.6f);
    }
    
    if (visible)
    {
        SDL_DisplayMode dm{};
        SDL_GetCurrentDisplayMode(0, &dm);

        RECT wr{};
        GetWindowRect(hwnd, &wr);
        configPositionX = wr.left;
        configPositionY = wr.top;
        configWidth     = wr.right  - wr.left;
        configHeight    = wr.bottom - wr.top;

        SDL_SetWindowBordered(window, SDL_FALSE);

        SetWindowPos(
            hwnd,
            HWND_TOPMOST,
            0, 0, dm.w, dm.h,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW
        );
    }
    else
    {
        // SDL_SetWindowBordered internally calls SetWindowPos with SWP_SHOWWINDOW,
        // which pulls the window to the foreground — so we explicitly push it back
        // down immediately after
        SDL_SetWindowBordered(window, SDL_TRUE);

        SetWindowPos(
            hwnd,
            HWND_BOTTOM,          // send to bottom of z-order first
            configPositionX,
            configPositionY,
            configWidth,
            configHeight,
            SWP_FRAMECHANGED | SWP_NOACTIVATE
        );

        // Then make it non-topmost without changing z-order further
        SetWindowPos(
            hwnd,
            HWND_NOTOPMOST,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
        );
    }

    if (!visible) {
        SetAlpha(window, hwnd, 1.0f);
    }
}
 
void App::ShowWheel(std::vector<std::string> elements) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 screenCenter = ImVec2(
        viewport->Pos.x + viewport->Size.x * 0.5f,
        viewport->Pos.y + viewport->Size.y * 0.5f
    );

    wheelSelectedIndex = WheelUI::RenderSelectionWheel(elements, wheelIndex, wheelVisible);
}


void App::UI()
{
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(w, h));

    ImGui::Begin(wheelVisible ? "Wheel" : "Soundboard", nullptr, flags);

    if (wheelVisible)
    {
        std::vector<Audio>* wheelAudios = soundboard.GetAudiosAtIndex(wheelIndex);

        std::vector<std::string> names;
        names.reserve(wheelAudios->size());

        for (const Audio& a : *wheelAudios)
            names.push_back(a.name);

        ShowWheel(names);
        ImGui::End();
        return;
    }

    ConfigurationUI::Render(soundboard);
    ImGui::End();
}

void App::InitializeUI() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cout << "SDL Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        "Soundboard",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_SysWMinfo wmInfo{};
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    hwnd = wmInfo.info.win.window;

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    int display_w, display_h;
    SDL_GL_GetDrawableSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

   bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                running = false;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                int w = event.window.data1;
                int h = event.window.data2;
                glViewport(0, 0, w, h);
            }
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        UI();

        ImGui::Render();

        SDL_GL_GetDrawableSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

   

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


App::App() {
    configManager = new ConfigManager(soundboard);
}

void App::HandleEvent(const Event& event) {
    if (event.getType() == EventType::ToggleWheel){
        if (soundboard.AudioTableSize() == 0) {
            return;
        }
        const ToggleWheelEvent& toggleWheelEvent = dynamic_cast<const ToggleWheelEvent&>(event);
        if (wheelVisible == toggleWheelEvent.visible) {
            return;
        }
        
        ToggleWheelVisible(toggleWheelEvent.visible);
        if (!wheelVisible) {
            soundboard.PlayAudio(wheelIndex, wheelSelectedIndex);
        }
    } else if (event.getType() == EventType::WheelScroll) {
        unsigned int audioTableSize = soundboard.AudioTableSize();
        if (audioTableSize <= 1) {
            return;
        }
        const WheelScrollEvent& wheelScrollEvent = dynamic_cast<const WheelScrollEvent&>(event);
        int deltaWheelIndex = wheelScrollEvent.scrollDirection ? 1 : -1;
        wheelIndex = (wheelIndex + deltaWheelIndex + audioTableSize) % audioTableSize;
    }
};


void App::Initialize() {
    InputListener::Start();
    EventDispatcher::Subscribe([this](const Event& event) -> void {
        HandleEvent(event);
    });
    InitializeUI();
    InputListener::Stop();
    return;
}

