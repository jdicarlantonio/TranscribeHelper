/*
  ==============================================================================

    BiQuad.h
    Created: 5 Sep 2018 6:46:55pm
    Author:  jd

  ==============================================================================
*/

#pragma once


#include <iostream>

static constexpr double PI = 3.14159;

enum FilterType
{
	PEAK, // 0
	LOW_PASS,
	HIGH_PASS,
	OTHER
};

class BiQuad
{
public:
	BiQuad();
	BiQuad(FilterType ftype); // 0 Peak, 1 Low pass, 2 High Pass
	~BiQuad();

	void calculateCoefficients(float fs, float f0, float gain = 0, float Q = 2);
	void process(float* buffer, float numSamples);
	float process(float sampleData);

	void setLowPass()
	{
		type = FilterType::LOW_PASS;
	}
	void setHighPass()
	{
		type = FilterType::HIGH_PASS;
	}

	FilterType getType()
	{
		return type;
	}

	void reset()
	{
		xn_1 = xn_2 = yn_1 = yn_2 = 0;
	}

	void changeType(FilterType ftype)
	{
		type = ftype;
	}

private:
	FilterType type;

	// Filter Coefficients
	float a1, a2;
	float b0, b1, b2;

	// Delay 
	float xn_1, xn_2;
	float yn_1, yn_2;
};