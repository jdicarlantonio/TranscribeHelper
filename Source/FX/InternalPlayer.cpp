#include "InternalPlayer.h"

InternalPlayer::InternalPlayer(AudioTransportSource& referenceSource)
	: transportSource(referenceSource)
{
}

void InternalPlayer::prepareToPlay(double sampleRate, int samplesPerBlock)
{
//	transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void InternalPlayer::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	transportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
}
