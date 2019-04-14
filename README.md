# TranscribeHelper

This is a simple application that can be used to help with transcribing parts of a song. Basically it loads a single audio file which can be processed. It also has functionality to loop through different sections. I made this mostly for transcribing guitar solos so initially all I wanted was a way to slow down the tempo and center in on some frequencies. This may continue into something more useful after I have learned a bit more, but maybe not.

Built using the JUCE framework.

### Built-in Effects

The built-in effects are built as plugins which uses the GenericAudioProcessorEditor class for the GUI. So you just get a bunch of sliders and stuff.

Current plans for FX:

* Four band equalizer with a low/high pass and one peak filter (implemented)
* Phase Vocoder for pitch shifting and time stretching (TODO) note: also considering using granular synthesis for this


