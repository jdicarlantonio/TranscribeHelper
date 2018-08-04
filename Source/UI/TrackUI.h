/*
  ==============================================================================

	TrackUI.h
	Created: 2 Aug 2018 2:51:44am
	Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

// user include
#include "TrackThumbnail.h"

//==============================================================================
/*
*/

class TrackUI   
	: public Component
	, public ActionBroadcaster
{
public:
	TrackUI();
	~TrackUI();

	void paint (Graphics&) override;
	void resized() override;

	void changeOpenButtonText(String& filename);
	void changeTime(String displayTime);
	void loop();

	bool isLooping() { return looping; }

private:
	TextButton openButton;
	TextButton loopButton;

	Label currentTimeLabel;

	bool looping;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackUI)
};
