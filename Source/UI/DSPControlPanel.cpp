/*
  ==============================================================================

    DSPControlPanel.cpp
    Created: 2 Aug 2018 2:07:44am
    Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "DSPControlPanel.h"

//==============================================================================
DSPControlPanel::DSPControlPanel()
{
	addAndMakeVisible(timeStretchSlider);
	timeStretchSlider.setSliderStyle(Slider::LinearVertical);
	timeStretchSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	timeStretchSlider.setColour(Slider::thumbColourId, Colours::black);
	timeStretchSlider.setPopupDisplayEnabled(true, false, this);

	addAndMakeVisible(pitchShiftSlider);
	pitchShiftSlider.setSliderStyle(Slider::LinearVertical);
	pitchShiftSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	pitchShiftSlider.setColour(Slider::thumbColourId, Colours::black);
	pitchShiftSlider.setPopupDisplayEnabled(true, false, this);


	addAndMakeVisible(tempoLabel);
	tempoLabel.setText("Tempo", dontSendNotification);
	tempoLabel.setColour(Label::textColourId, Colours::black);
	tempoLabel.setJustificationType(Justification::horizontallyCentred);

	addAndMakeVisible(pitchLabel);
	pitchLabel.setText("Pitch", dontSendNotification);
	pitchLabel.setColour(Label::textColourId, Colours::black);
	pitchLabel.setJustificationType(Justification::horizontallyCentred);
}

DSPControlPanel::~DSPControlPanel()
{
}

void DSPControlPanel::paint (Graphics& g)
{
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour (Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
} 

void DSPControlPanel::resized()
{
	float numSliders = 2;
	auto panelX = getLocalBounds().getX();
	auto panelY = getLocalBounds().getY();
	auto panelWidth = getLocalBounds().getWidth();
	auto panelHeight = getLocalBounds().getHeight();

	auto sliderWidth = panelWidth / (2.0f * numSliders);
	float labelSize = 30;

	timeStretchSlider.setBounds(
		panelX + sliderWidth,
		panelY,
		sliderWidth,
		panelHeight - labelSize
	);

	tempoLabel.setBounds(
		panelX + sliderWidth,
		panelHeight - labelSize,
		sliderWidth,
		labelSize
	);

	pitchShiftSlider.setBounds(
		panelX + (2 * sliderWidth),
		panelY,
		sliderWidth,
		panelHeight - labelSize
	);

	pitchLabel.setBounds(
		panelX + (2 * sliderWidth),
		panelHeight - labelSize,
		sliderWidth,
		labelSize
	);
}