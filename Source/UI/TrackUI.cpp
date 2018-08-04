/*
  ==============================================================================

    TrackUI.cpp
    Created: 2 Aug 2018 2:51:44am
    Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TrackUI.h"

//==============================================================================
TrackUI::TrackUI()
	: looping(false)
{
	addAndMakeVisible(openButton);
	openButton.setButtonText("Open...");
	openButton.setColour(TextButton::buttonColourId, Colours::ghostwhite);
	openButton.setColour(TextButton::textColourOffId, Colours::black);

	addAndMakeVisible(loopButton);
	loopButton.setButtonText("Loop");
	loopButton.setColour(TextButton::buttonColourId, Colours::green);
	loopButton.setColour(TextButton::textColourOffId, Colours::white);

	addAndMakeVisible(currentTimeLabel);
	currentTimeLabel.setColour(Label::backgroundColourId, Colours::darkgrey);
	currentTimeLabel.setColour(Label::textColourId, Colours::ghostwhite);
	currentTimeLabel.setJustificationType(Justification::horizontallyCentred);
	currentTimeLabel.setText("00:00", dontSendNotification);

	openButton.onClick = [this] { sendActionMessage("open"); };
	loopButton.onClick = [this] { sendActionMessage("loop"); loop(); };
}

TrackUI::~TrackUI()
{
}

void TrackUI::paint (Graphics& g)
{
//    g.fillAll ();   // clear the background

//    g.setColour (Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void TrackUI::resized()
{
	auto panelX = getLocalBounds().getX();
//	auto panelY = getLocalBounds().getY();
	auto panelWidth = getLocalBounds().getWidth();
	auto panelHeight = getLocalBounds().getHeight();

	openButton.setBounds(
		panelX,
		panelHeight / 4,
		panelWidth / 8,
		panelHeight / 12
	);

	loopButton.setBounds(
		panelWidth - (panelWidth / 8),
		panelHeight / 4,
		panelWidth / 8,
		panelHeight / 12
	);

	auto labelWidth = panelWidth / 12;
	auto labelHeight = panelHeight / 12;

	currentTimeLabel.setFont(Font(labelHeight, Font::bold));

	currentTimeLabel.setBounds(
		panelX,
		panelHeight - labelHeight,
		labelWidth,
		labelHeight
	);
}

void TrackUI::changeOpenButtonText(String& filename)
{
	openButton.setButtonText(filename);
}

void TrackUI::changeTime(String displayTime)
{
	currentTimeLabel.setText(displayTime, dontSendNotification);
}

void TrackUI::loop()
{
	looping = !looping;
	if(looping)
	{
		loopButton.setColour(TextButton::buttonColourId, Colours::red);
	}
	else
	{
		loopButton.setColour(TextButton::buttonColourId, Colours::green);
	}
}
