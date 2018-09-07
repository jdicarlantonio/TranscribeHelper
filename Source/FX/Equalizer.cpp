/*
  ==============================================================================

    Equalizer.cpp
    Created: 5 Sep 2018 7:15:06pm
    Author:  jd

  ==============================================================================
*/

#include "Equalizer.h"

Equalizer::Equalizer()
{
	addParameter(peakQ = new AudioParameterFloat(
		"peakQ",
		"Filter Q",
		0.100f,
		2.000f,
		0.707f)
	);
	addParameter(peakFreq = new AudioParameterFloat(
		"peakFreq",
		"Filter f0",
		10.00f,
		20000.00f,
		7000.00f)
	);
	addParameter(peakGain = new AudioParameterFloat(
		"peakGain",
		"Filter dB",
		-18.00f,
		18.00f,
		0.00f)
	);

	addParameter(lpfQ = new AudioParameterFloat(
		"lpfQ",
		"LPF Q",
		0.100f,
		2.000f,
		0.707f)
	);
	addParameter(lpfFreq = new AudioParameterFloat(
		"lpfFreq",
		"LPF f0",
		10.00f,
		20000.00f,
		15000.00f)
	);

	addParameter(hpfQ = new AudioParameterFloat(
		"hpfQ",
		"HPF Q",
		0.100f,
		2.000f,
		0.707f)
	);
	addParameter(hpfFreq = new AudioParameterFloat(
		"hpfFreq",
		"HPF f0",
		10.00f,
		20000.00f,
		50.00f)
	);

	leftLPF.setLowPass();
	rightLPF.setLowPass();

	leftHPF.setHighPass();
	rightHPF.setHighPass();
}

Equalizer::~Equalizer()
{
}

void Equalizer::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	leftFilter.reset();
	rightFilter.reset();
}

void Equalizer::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
	float pQ = peakQ->get();
	float pFreq = peakFreq->get();
	float pGain = peakGain->get();

	/*
	float lFreq = pFreq + 1000.0f;
	float hFreq = pFreq - 1000.0f;
	*/

	float lQ = lpfQ->get();
	float lFreq = lpfFreq->get();
	
	float hQ = hpfQ->get();
	float hFreq = hpfFreq->get();

	leftFilter.calculateCoefficients(
		getSampleRate(),
		pFreq,
		pGain,
		pQ
	);
	rightFilter.calculateCoefficients(
		getSampleRate(),
		pFreq,
		pGain,
		pQ
	);

	leftLPF.calculateCoefficients(
		getSampleRate(),
		lFreq,
		pGain,
		lQ
	);
	rightLPF.calculateCoefficients(
		getSampleRate(),
		lFreq,
		pGain,
		lQ
	);

	leftHPF.calculateCoefficients(
		getSampleRate(),
		hFreq,
		pGain,
		hQ
	);
	rightHPF.calculateCoefficients(
		getSampleRate(),
		hFreq,
		pGain,
		hQ
	);

	float* leftChannelData = buffer.getWritePointer(0);
	float* rightChannelData = buffer.getWritePointer(1);

	leftFilter.process(leftChannelData, buffer.getNumSamples());
	leftLPF.process(leftChannelData, buffer.getNumSamples());
	leftHPF.process(leftChannelData, buffer.getNumSamples());

	rightFilter.process(rightChannelData, buffer.getNumSamples());
	rightLPF.process(rightChannelData, buffer.getNumSamples());
	rightHPF.process(rightChannelData, buffer.getNumSamples());
}



