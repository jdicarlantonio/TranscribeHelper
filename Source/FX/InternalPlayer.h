/*
  ==============================================================================

    InternalPlugin.h
    Created: 13 Aug 2018 4:17:17pm
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../ProcessorBase.h"

// Internal plugin to play an audio file
class InternalPlayer
	: public ProcessorBase
{
public:
	InternalPlayer(AudioTransportSource& referenceSource);
	~InternalPlayer() {}

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override {}
	void processBlock(AudioSampleBuffer& buffer, MidiBuffer&) override;
private:
	AudioTransportSource& transportSource;
};