cmake_minimum_required(VERSION 3.15)

project(Soundboard)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(NOT DEFINED VCPKG_ROOT OR VCPKG_ROOT STREQUAL "")
    set(VCPKG_ROOT "D:/dev/vcpkg" CACHE PATH "Path to vcpkg root")
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET OR VCPKG_TARGET_TRIPLET STREQUAL "")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet")
endif()


# ---- Find packages installed by vcpkg ----
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# ---- External subdirectories ----
add_subdirectory(external/nativefiledialog-extended)
add_subdirectory(external/json)
add_subdirectory(external/libuiohook)
# ---- Your executable ----
add_executable(Soundboard
    # Core
    src/core/main.cpp
    src/core/app.cpp
    # UI
    src/ui/file_dialog.cpp
    src/ui/wheel_UI.cpp
    src/ui/configuration_UI.cpp
    # Input
    src/input/input_manager.cpp
    src/input/hotkey_manager.cpp
    src/input/hotkey_translator.cpp
    # Event
    src/event/event_dispatcher.cpp
    src/event/event.cpp
    # Audio
    src/audio/audio_manager.cpp
    # Soundboard
    src/soundboard/soundboard.cpp
    src/soundboard/audio_table.cpp
    #config
    src/config/config_manager.cpp

    # DearImGUI
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp

    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    #external/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

target_compile_definitions(Soundboard PRIVATE SDL_MAIN_HANDLED)

# ---- Link libraries ----
target_link_libraries(Soundboard PRIVATE
    SDL2::SDL2
    nlohmann_json::nlohmann_json
    nfd
    OpenGL::GL
    uiohook
)

target_include_directories(Soundboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends

    ${CMAKE_CURRENT_SOURCE_DIR}/external/nativefiledialog-extended/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/miniaudio
    
)