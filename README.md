# Simple Soundboard
Simple Soundboard is a lightweight Windows soundboard application that allows audio files to be played through a configurable audio output device using an emote-wheel-style UI and global hotkeys.

The application combines real-time audio playback, configurable audio devices, global input handling, a transparent fullscreen overlay, persistent configuration, and a custom wheel-based UI into a small native desktop application.

[See example screenshots](#screenshots)

### Supports two primary controls:

* **Wheel Hotkey** — Opens and closes the sound selection wheel.
* **Stop Hotkey** — Stops all currently playing sounds.

## Quick start
1. Download a [release](https://github.com/DevManDan1178/simple-soundboard/releases) (ZIP) and extract it
2. Launch `SimpleSoundboard.exe`
      - If it is blocked by Windows Defender, you will have to allow it to run
3. Configure your preferred settings
      - Desired Hotkeys
      - Separate Output Audio Driver
      - Master Volume
      - Speaker Volume Modifier
      - Audio Drivers
4. Add a sound wheel
5. Add audio files to the wheel (select already downloaded audio files from your PC)
6. Play a sound!
      1. Open the wheel by clicking and holding the hotkey.
      2. Select a sound by hovering with mouse,
      3. Let go of the hotkey to confirm your choice.

## Features
* Emote-wheel-style audio selection UI.
* Support for multiple configurable sound wheels.
* Up to 6 audio files per wheel.
* Play and stop multiple sounds simultaneously.
* Global hotkeys for opening the wheel and stopping playback.
* Scroll wheel support for switching between sound wheels.
* Per-audio volume and sound effect configuration.
* Master volume control.
* Independent local playback volume.
* Configurable audio output device.
* Native file dialog for selecting audio files.
* Persistent configuration.
* Semi-transparent fullscreen overlay.
* Support for common audio formats supported by the audio backend.

### Playing Audio From a Wheel
Audios to play are selected from an emote-wheel-style UI. 

The would-be selected audio in the wheel is determined by the position of the mouse on the user's screen and will be highlighted.

In order to close the wheel without playing a sound, the user simply has to keep their mouse in the middle of the wheel, avoiding the selection of an audio to play.

*The wheel is rendered as a semi-transparent fullscreen overlay, allowing the wheel to be displayed over the current application without requiring the user to switch windows.*
### Managing Wheels
Audio files are binded to wheels, which are then operated like an emote wheel to select and play the audio.

- Wheels can be added or deleted (for which all audios of that wheel are also removed) in the configuration UI.
- Maximum of 6 audios per wheel.
- Audios can be picked from any path in your PC.

An audio driver can be picked for which the soundboard application will replay the audio in.

*The use of a third party app to mix the input of audio drivers is necessary, like <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.*

### Audio Configurations
Each audio slot can be configured individually with a specific audio file and volume.

The application also provides global volume controls:
- Master volume
- Speaker Volume Modifier (volume multiplier for the audio replayed to you)

### Audio Routing
Besides the default audio playback device, the soundboard can send audio to another Windows audio output device selected by the user.

The selected device can be changed through the configuration UI.

The application maintains separate playback paths for routed and local audio, allowing soundboard audio to be sent to an output device while also providing local playback.

### Audio Playback Alongside a Microphone Device
The soundboard cannot directly write audio into an existing microphone input driver.

If the goal is to combine soundboard audio with microphone audio and then send both to another application, an external audio mixer or virtual audio device is required.

A decent option (that I use myself) is [VoiceMeeter](https://vb-audio.com/Voicemeeter/).

#### Using With Voicemeeter
After the VoiceMeeter installation and the required PC reboot, open VoiceMeeter.

To output the audio to VoiceMeeter for audio driver mixing
- select the `VoiceMeeter Input` audio driver.
- in the VoiceMeeter UI, add your microphone as an input device.

Your selected audio driver in the configuration UI should look like this

<img width="498" height="61" alt="image" src="https://github.com/user-attachments/assets/a2fedd11-3ec9-44bb-8702-0494583cc653" />


Your VoiceMeeter UI should look like this

<img width="515" height="310" alt="image" src="https://github.com/user-attachments/assets/bd671ef8-ff53-4a9b-8036-07e81b00a9f1" />


You're all set! You can now select Voicemeeter's output (`CABLE Output`) driver and use it for voice chat.

## Usage
1. Launch SimpleSoundboard.exe
      - Make sure VoiceMeeter or another audio driver mixer is open and working if planning to mix driver audios.
2. Configure your preferred settings
      - Desired Hotkeys
      - Separate Output Audio Driver
      - Master Volume
      - Speaker Volume Modifier
      - Audio Drivers
3. Create one or more sound wheels.
4. Add audio files to each wheel.
5.  Play a sound!
      1. Open the wheel by clicking and holding the hotkey.
      2. Select a sound by hovering with mouse,
      3. Let go of the hotkey to confirm your choice.
6. Use the `Stop All Sounds` hotkey to stop all currently playing sounds.

## Screenshots
### Example Configuration UI
<img width="610" height="640" alt="image" src="https://github.com/user-attachments/assets/947afa3d-52c6-454b-a0a8-ff13eab04dfa" />

### Example Wheel Overlay

<img width="560" height="510" alt="image" src="https://github.com/user-attachments/assets/5051c496-4703-4e46-9143-ac1fec65c969" />


*This screenshot was taken with the wheel UI opened in front of [this image](https://www.w3schools.com/colors/img_colormap.gif)*

<img width="268" height="244" alt="image" src="https://github.com/user-attachments/assets/48d9b1ea-dd17-4c8c-9399-19bf2413c832" />


<br><br>


# Technical Overview
## How It Works
Simple Soundboard is divided into several components responsible for application lifecycle, audio playback, input handling, configuration, events, and UI.
At a high level, the application follows this workflow:
```
                   Global Input
                       |
                       v
                Hotkey / Mouse Event
                       |
                       v
                  Event System
                       |
          +------------+------------+
          |                         |
          v                         v
      Wheel UI                 Stop Playback
          |
          v
    Select Audio
          |
          v
    Audio Manager
          |
     +----+----+
     |         |
     v         v
   Local     Routed
 Playback   Playback
     |         |
     v         v
 Local     Selected
 Device    Device
```
The UI is responsible for presenting the wheel and configuration interfaces, while the audio and input systems handle playback and global input independently from the presentation layer.

### Audio Playback
Audio playback is handled using the `miniaudio` library.

The audio system supports separate playback paths for local and routed audio.

The audio manager is responsible for
- Initializing audio playback.
- Loading audio files.
- Managing playback of sounds.
- Applying volume settings.
- Applying audio effects.
- Managing the selected output device.
- Reinitializing routed playback when the output device changes.
- Stopping active playback.

This allows the application to continue using the configured soundboard while keeping local playback behavior separate from routed playback.

### Audio Device Selection
The application enumerates available Windows playback devices and allows one to be selected as the soundboard's routed output.

The audio system supports Windows audio backends including
- WASAPI
- DirectSound
- WinMM
When the selected device changes, the routed audio playback system is reconfigured for the new device.

## Global Input
Global keyboard and mouse input is handled using the `libuiohook` library, allowing the application to detect configured hotkeys while another application has focus.

### Wheel UI
The wheel UI is implemented using Dear ImGui with SDL2 and OpenGL.

The wheel is displayed as a transparent fullscreen overlay.

When opened, the wheel divides the available selection area into audio slots. Moving the mouse toward a slot selects the corresponding sound.

The overlay is designed to remain visually lightweight so that the application underneath remains visible while the sound is being selected.

### Configuration
Configuration is persisted using JSON.

The configuration system stores the application states below, allowing it to restore the user's soundboard configuration after restarting.
- Sound wheels
- Audio file paths
- Audio settings
- Volume settings
- Selected output driver
- Hotkeys

## Tech Stack

* **Language:** C++20
* **Build System:** CMake
* **UI:** Dear ImGui
* **Windowing:** SDL2
* **Graphics:** OpenGL
* **Audio:** miniaudio
* **Global Input:** libuiohook
* **File Dialog:** nativefiledialog-extended
* **Configuration:** nlohmann/json
* **Package Management:** vcpkg

The project is built as a native Windows desktop application using C++20 and CMake.

## Architecture
The source code is separated into components based on responsibility    
```
src/
├── audio/
│   └── Audio playback and device management
│
├── config/
│   └── Configuration persistence
│
├── core/
│   └── Application lifecycle and entry point
│
├── event/
│   └── Application event handling
│
├── input/
│   └── Global keyboard and mouse input
│
├── soundboard/
│   └── Soundboard and audio data management
│
└── ui/
    └── Configuration and wheel interfaces
```

## Limitations
- The use of a third party app to combine audio from a microphone driver and a seperate driver is required.
    - The soundboard cannot play sound into existing microphone audio drivers, as they are read-only.
    - Recommended: <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.
  
*To trackpad users: get a mouse pls*

## Engineering Challenges
### Routing Audio to a Selected Device

The application needs to send soundboard audio to a specific output device rather than simply relying on the system's default audio output.

The audio system therefore needs to enumerate available devices, identify the configured device, and initialize playback against the selected device.

Changing the selected device also requires the routed audio playback system to be reconfigured.

### Simultaneous Audio Playback
A soundboard needs to allow multiple sounds to play at the same time.

Starting a new sound should not interrupt sounds that are already playing, allowing multiple audio effects to overlap naturally.

This requires the audio system to manage multiple active sounds while keeping the application responsive.

### Separating Local and Routed Playback
The application has two different playback requirements:

1. Audio that should be heard locally.
2. Audio that should be sent through the configured soundboard output.

Keeping these playback paths separate allows local monitoring to have its own volume control while routed audio can be sent to the configured output device.

### Global Input Without Application Focus
The soundboard is primarily intended to be used while another application has focus.

Normal application-local keyboard events would therefore not be sufficient for the main controls.

Global input handling allows the application to detect the configured hotkeys while the user is interacting with another application.

### Transparent Overlay UI
The wheel needs to appear over the application currently being used without requiring the user to switch windows.

The UI is therefore implemented as a transparent fullscreen overlay that can be opened and closed using the configured global hotkey.

The overlay also needs to remain lightweight enough that the application underneath remains visible while selecting a sound.

### Persisting User Configuration

Users should not need to configure their soundboard every time the application starts.

The configuration system persists the user's sound wheels, audio files, audio settings, volume settings, selected output device, and hotkeys.

This allows the application to restore the soundboard's previous state when it is launched again.

### Managing Audio Device Changes
The selected output device can change while the application is running.

The audio system therefore needs to handle changing the configured device and reinitialize routed playback so that subsequent sounds are sent to the newly selected device.

### Keeping Input, Audio, and UI Decoupled
The application needs to coordinate global input, audio playback, configuration, and UI state without making each system directly dependent on the others.

An event-based structure is used to communicate actions between components.

This allows, for example, a global hotkey event to be handled without the input system needing to directly manage the audio playback implementation or UI rendering.

## Design Decisions
### Separated Local and Routed Audio Engines
Local and routed playback are kept separate rather than treating them as a single output.

This allows the local audio to have its own volume control while the routed audio can still be sent unchanged to the configured output device.

### Wheel-Based Selection
In order to avoid the user requiring to memorize every audio to their configured hotkey, I decided on making it a popup UI overlay on hotkey press to minimize the amount of required configurable hotkeys.

A wheel interface was chosen instead of a traditional list or grid because it is very accessible and intuitive for most users.

The amount of audios in a wheel was also capped to avoid cases where narrow slices can cause the user to select the wrong audio.

### Event-Based Application Structure
Input actions and other application changes are communicated through the application's event system rather than requiring every component to directly control every other component.

This reduces coupling between the UI, input, and soundboard systems.


