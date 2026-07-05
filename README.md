# Crappy Soundboard

This is a small soundboard application that lets you play (and stop) audio files with an emote wheel style UI.


## 2 (+2) Hotkeys Only
- One for opening/closing the wheel UI
- One for stopping the sounds that are playing
- Scroll wheel up/down for switching between wheels I guess

## Setup

Audio files are binded to wheels, which are then operated like an emote wheel to select and play the audio.

- *Wheels can be added or deleted (for which all audios of that wheel are also removed).*
- *Maximum of 6 audios per wheel.*

- *Audios are picked through a file dialog, so you can pick any audio (mp3, wav...) from the file explorer.*

An audio driver can be picked for which the soundboard application will replay the audio in.

*The use of a third party app to mix the input of audio drivers is necessary, like <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.*

### Adjustable Parameters
- Audio sound effects (individual per audio)
- Master volume
- Local volume (volume multiplier for the audio replayed to you)
- Keybinds
  - Toggling the wheel UI
  - Stopping all sounds


*To trackpad users: get a mouse pls*

## Limitations
- The use of a third party app to combine audio from a microphone driver and a seperate driver is required.
    - The soundboard cannot play sound into existing microphone audio drivers, as they are read-only.
    - Recommended: <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.
