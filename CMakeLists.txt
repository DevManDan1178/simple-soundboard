cmake_minimum_required(VERSION 3.15)

project(Soundboard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(NOT DEFINED VCPKG_ROOT OR VCPKG_ROOT STREQUAL "")
    set(VCPKG_ROOT "D:/dev/vcpkg" CACHE PATH "Path to vcpkg root")
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET OR VCPKG_TARGET_TRIPLET STREQUAL "")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet")
endif()


# ---- Find packages installed by vcpkg ----
find_package(SDL2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


# ---- External subdirectories ----
add_subdirectory(external/nativefiledialog-extended)

# ---- Your executable ----
add_executable(Soundboard
    # Core
    src/core/main.cpp
    src/core/app.cpp
    # UI
    src/ui/file_dialog.cpp
    src/ui/wheel_overlay_UI.cpp
    src/ui/configuration_UI.cpp
    # Input
    src/input/input_manager.cpp
    # Event
    src/event/event_dispatcher.cpp
    src/event/event.cpp
    # Audio
    src/audio/audio_manager.cpp
    src/audio/audio.cpp
)


# ---- Link libraries ----
target_link_libraries(Soundboard PRIVATE
    SDL2::SDL2
    nlohmann_json::nlohmann_json
    imgui::imgui
    nfd
)

target_include_directories(Soundboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nativefiledialog-extended/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/miniaudio
)