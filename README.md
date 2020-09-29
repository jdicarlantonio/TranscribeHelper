# TranscribeHelper

This is a simple application that can be used to help with transcribing parts of a song. Basically it loads a single audio file which can be processed. It also has functionality to loop through different sections. Scrolling the mouse wheel down on the waveform will place the marker at the start of where you want to loop (green marker) and scrolling up will place the marker at the end of the section (dark red marker). You can click anywhere on the waveform to move your current position on the track (light red marker).

Built using the JUCE framework.

![ScreenShot](https://raw.githubusercontent.com/jdicarlantonio/TranscribeHelper/master/Screenshot/transcribeScreenshot.png)

### Built-in Effects

The built-in effects (not pictured) are built as plugins which uses the GenericAudioProcessorEditor class for the GUI. So you just get a bunch of sliders and stuff.

### In Progress

Currently porting this to use the newest version of JUCE. The UI and playing tracks is fully functional, but a lot of the effects code is now unsupported.
