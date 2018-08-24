# TranscribeHelper
This is not a full on automatic music transcriber, it is simply an audio player with some features (so far just time-stretching, pitch shifting, and looping is on the agenda). Built using the JUCE framework (https://juce.com/discover).

Current Functionality:
  Just plays and loops different sections of a song. Just implemented the ability to add 3rd party plugins, however, right now there is no plugin window and you are unable to remove plugins.

## TODO: 
* Implement home and end functionality.
* Get the AudioProcessorGraph fully working (add ability to remove plugins and get a plugin window to display plugin GUI)
* Implement built in plugins: time-stretching/pitch-shifting (I am undecided between a phase vocoder or granular synthesis for this) and equalizer.
