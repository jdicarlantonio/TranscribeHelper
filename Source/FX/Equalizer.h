/*
  ==============================================================================

    Equalizer.h
    Created: 5 Sep 2018 7:15:06pm
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "ProcessorBase.h"
#include "BiQuad.h"

// butterworth bandpass filter for singling in on a certain freq
class Equalizer
	: public ProcessorBase
{
public:
	Equalizer();
	~Equalizer();

	// plugin overrides
	//==============================================================================

	void prepareToPlay(double, int) override;
	void processBlock(AudioSampleBuffer& buffer, MidiBuffer&) override;

	int getNumPrograms() override { return 1; }

	const String getName() const override { return "Equalizer"; }

	//==============================================================================

private:
	// plugin parameters
	AudioParameterFloat* peakQ;
	AudioParameterFloat* peakFreq;
	AudioParameterFloat* peakGain;

	AudioParameterFloat* lpfQ;
	AudioParameterFloat* lpfFreq;

	AudioParameterFloat* hpfQ;
	AudioParameterFloat* hpfFreq;

	BiQuad leftFilter;
	BiQuad rightFilter;

	BiQuad leftLPF;
	BiQuad rightLPF;

	BiQuad leftHPF;
	BiQuad rightHPF;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Equalizer)
};
