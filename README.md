# Simple Soundboard
Simple Soundboard is a lightweight Windows soundboard application built for Windows that allows audio files to be played through a configurable audio output device using an emote-wheel-style UI and global hotkeys.

The application combines real-time audio playback, configurable audio devices, global input handling, a transparent fullscreen overlay, persistent configuration, and a custom wheel-based UI into a small native desktop application.

### Supports two primary controls:

* **Wheel Hotkey** — Opens and closes the sound selection wheel.
* **Stop Hotkey** — Stops all currently playing sounds.

## Quick start
1. Click on `Releases`
2. Download a release (ZIP) and extract it
3. Launch `SimpleSoundboard.exe`
4. Open the configuration UI
5. Select an audio output device
6. Create a sound wheel
7. Add audio files to the wheel (select already downloaded audio files from your PC)
8. Configure the desired hotkeys
9. Open the wheel and select a sound

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

#### Using Voicemeeter
To output the audio to VoiceMeeter for audio driver mixing:
- select the `VoiceMeeter Input` audio driver.
- in the VoiceMeeter UI, add your microphone as an input device.

Your selected audio driver in the configuration UI should look like this

<img width="498" height="61" alt="image" src="https://github.com/user-attachments/assets/a2fedd11-3ec9-44bb-8702-0494583cc653" />


Your VoiceMeeter UI should look like this

<img width="515" height="310" alt="image" src="https://github.com/user-attachments/assets/bd671ef8-ff53-4a9b-8036-07e81b00a9f1" />


You're all set! You can now select Voicemeeter's output (`CABLE Output`) driver and use it for voice chat.

## Usage
1. Launch SimpleSoundboard.exe.
2. Open the configuration UI.
3. Select the desired audio output device.
4. Create one or more sound wheels.
5. Add audio files to each wheel.
6. Configure audio effects and volume.
7. Configure the wheel and stop hotkeys.
8. Open the wheel using the configured hotkey.
9. Select a sound using the mouse.
10. Scroll to switch between wheels when necessary.
11. Use the `Stop All Sounds` hotkey to stop all currently playing sounds.

## Screenshots
### Example Configuration UI
<img width="610" height="640" alt="image" src="https://github.com/user-attachments/assets/947afa3d-52c6-454b-a0a8-ff13eab04dfa" />

### Example Wheel

<img width="748" height="686" alt="image" src="https://github.com/user-attachments/assets/5051c496-4703-4e46-9143-ac1fec65c969" />


*This screenshot was taken with the wheel UI opened in front of [this image](https://www.w3schools.com/colors/img_colormap.gif)*

<img width="268" height="244" alt="image" src="https://github.com/user-attachments/assets/48d9b1ea-dd17-4c8c-9399-19bf2413c832" />


<br><br>




## Limitations
- The use of a third party app to combine audio from a microphone driver and a seperate driver is required.
    - The soundboard cannot play sound into existing microphone audio drivers, as they are read-only.
    - Recommended: <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.
  
*To trackpad users: get a mouse pls*
