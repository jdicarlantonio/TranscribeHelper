/*
  ==============================================================================

	SongControlPanel.h
	Created: 30 Jul 2018 10:29:40pm
	Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SongControlPanel    
	: public Component
	, public ActionBroadcaster
{
public:
	SongControlPanel();
	~SongControlPanel();

	void paint (Graphics&) override;
	void resized() override;

	void disableButtons(String state);
	double getGain() { return gainSlider.getValue(); }

private:
	ShapeButton stopButton;
	ShapeButton playButton;
	ShapeButton pauseButton;
	ShapeButton homeButton; // place cursor to beginning of track
	ShapeButton endButton;  // place cursor to end of track

	Slider gainSlider;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SongControlPanel)
};
