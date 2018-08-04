/*
  ==============================================================================

    DSPControlPanel.h
    Created: 2 Aug 2018 2:07:44am
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DSPControlPanel    : public Component
{
public:
    DSPControlPanel();
    ~DSPControlPanel();

    void paint (Graphics&) override;
    void resized() override;

private:
	// only time stretch and pitch shifting for right now
	// will need to eventually add an EQ with its own window
	Slider timeStretchSlider;
	Slider pitchShiftSlider;
	// TODO: Customize slider lookandfeel

	Label tempoLabel;
	Label pitchLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPControlPanel)
};
