# Simple Soundboard

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


## Screenshots
### Example Configuration UI
<img width="305" height="320" alt="image" src="https://github.com/user-attachments/assets/947afa3d-52c6-454b-a0a8-ff13eab04dfa" />

### Example Wheel
<img width="292" height="287" alt="image" src="https://github.com/user-attachments/assets/d7ce2030-8eff-413c-9432-36c85e7da861" />

<br><br>

*This screenshot was taken with the wheel UI opened on a fully white background window.*

*The wheel overlays on the entire screen but is semi-transparent and should not significantly impact the visibility of the current window.*

## Limitations
- The use of a third party app to combine audio from a microphone driver and a seperate driver is required.
    - The soundboard cannot play sound into existing microphone audio drivers, as they are read-only.
    - Recommended: <a href="https://vb-audio.com/Voicemeeter/">VoiceMeeter</a>.
  
*To trackpad users: get a mouse pls*
