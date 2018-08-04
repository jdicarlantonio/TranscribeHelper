/*
  ==============================================================================

    PositionOverlay.h
    Created: 3 Aug 2018 5:29:02am
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PositionOverlay    
	: public Component
	, private Timer
{
public:
	PositionOverlay(AudioTransportSource& transportSourceToUse);
    ~PositionOverlay();

    void paint (Graphics&) override;
    void resized() override;

	void mouseDown(const MouseEvent& event) override;
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

	void drawLoopOverlays(Graphics& g);
	void getLoopBounds(double& start, double& end);
	void clearLoopBounds() {loopStartTime = loopEndTime = 0.0; }
	void setLooping(bool isLooping) { looping = isLooping; }

private:
	void timerCallback() override
	{
		repaint();
	}

	AudioTransportSource& transportSource;

	double loopStartTime;
	double loopEndTime;
	double loopStartDrawPos;
	double loopEndDrawPos;

	bool looping;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};