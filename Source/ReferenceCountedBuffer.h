/*
  ==============================================================================

    ReferenceCountedBuffer.h
    Created: 3 Aug 2018 6:24:24am
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// pretty much the same implementation from the tutorial, I don't think I'll need
// anything fancy just yet
class ReferenceCountedBuffer
	: public ReferenceCountedObject
{
public:
	typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;

	ReferenceCountedBuffer(int numChannels, int numSamples)
		: buffer(numChannels, numSamples)
	{
	}
	~ReferenceCountedBuffer()
	{
	}

	AudioSampleBuffer* getAudioSampleBuffer()
	{
		return &buffer;
	}

	int position = 0;

private:
	AudioSampleBuffer buffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
};