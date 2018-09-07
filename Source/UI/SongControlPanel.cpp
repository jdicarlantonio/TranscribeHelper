/*
  ==============================================================================

	SongControlPanel.cpp
	Created: 30 Jul 2018 10:29:40pm
	Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SongControlPanel.h"

//==============================================================================
SongControlPanel::SongControlPanel()
	: playButton("Play", Colours::black, Colours::grey, Colours::dimgrey)
	, stopButton("Stop", Colours::black, Colours::grey, Colours::dimgrey)
	, pauseButton("Pause", Colours::black, Colours::grey, Colours::dimgrey)
	, homeButton("Home", Colours::black, Colours::grey, Colours::dimgrey)
	, endButton("end", Colours::black, Colours::grey, Colours::dimgrey)
{
	addAndMakeVisible(playButton);
	playButton.setEnabled(false);

	addAndMakeVisible(stopButton);
	stopButton.setEnabled(false);

	addAndMakeVisible(pauseButton);
	pauseButton.setEnabled(false);

	addAndMakeVisible(homeButton);
	addAndMakeVisible(endButton);

	addAndMakeVisible(gainSlider);
	gainSlider.setSliderStyle(Slider::LinearBarVertical);
	gainSlider.setColour(Slider::textBoxTextColourId, Colours::grey);
	gainSlider.setTextValueSuffix("dB");
	gainSlider.setNormalisableRange(NormalisableRange<double>(-40.0, 10.0, 0.1));
	gainSlider.setValue(0.0);

	playButton.onClick = [this] { sendActionMessage("play"); };
	stopButton.onClick = [this] { sendActionMessage("stop"); };
	pauseButton.onClick = [this] { sendActionMessage("pause"); };
	homeButton.onClick = [this] {sendActionMessage("home"); };
	endButton.onClick = [this] {sendActionMessage("end"); };
	gainSlider.onValueChange = [this] {sendActionMessage("gain"); };
}

SongControlPanel::~SongControlPanel()
{
}

void SongControlPanel::paint (Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/
//	g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

//	g.setColour (Colours::grey);
//	g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

	Path stopShape;
	Path playShape;
	Path pauseShape;
	Path homeShape, endShape;

	stopShape.addRoundedRectangle(
		stopButton.getX(),
		stopButton.getY(),
		stopButton.getWidth(),
		stopButton.getHeight(),
		10.0f
	);

	playShape.addPolygon(
		Point<float>(playButton.getWidth() / 2, playButton.getHeight() / 2),
		3,
		playButton.getHeight(),
		1.570796f // ~90 degrees
	);

	float pauseLineWidth = pauseButton.getWidth() * 0.10f; // 10 percent of the button
	pauseShape.addRoundedRectangle(
		pauseButton.getX(),
		pauseButton.getY(),
		pauseLineWidth,
		pauseButton.getHeight(),
		5.0f
	);

	pauseShape.addRoundedRectangle(
		pauseButton.getX() + (2 * pauseLineWidth), // one pause line between the two 
		pauseButton.getY(),
		pauseLineWidth,
		pauseButton.getHeight(),
		5.0f
	);

	homeShape.addPolygon(
		Point<float>(playButton.getWidth() / 2, playButton.getHeight() / 2),
		3,
		playButton.getHeight(),
		4.712389 // ~270 degrees
	);

	endShape.addPolygon(
		Point<float>(playButton.getWidth() / 2, playButton.getHeight() / 2),
		3,
		playButton.getHeight(),
		1.570796f // ~90 degrees
	);

	stopButton.setShape(stopShape, false, true, true);
	playButton.setShape(playShape, false, true, true);
	pauseButton.setShape(pauseShape, false, true, true);
	homeButton.setShape(homeShape, false, true, true);
	endButton.setShape(endShape, false, true, true);
}

void SongControlPanel::resized()
{
	// get local attributes
	auto panelX = getLocalBounds().getX();
	auto panelY = getLocalBounds().getY();
	auto buttonSize = getLocalBounds().getHeight();
	auto panelWidth = getLocalBounds().getWidth();

	stopButton.setBounds(
		panelX,
		panelY,
		buttonSize,
		buttonSize
	);

	playButton.setBounds(
		panelX + buttonSize,
		panelY,
		buttonSize,
		buttonSize
	);

	pauseButton.setBounds(
		panelX + (2 * buttonSize),
		panelY,
		buttonSize,
		buttonSize
	);

	homeButton.setBounds(
		panelX + (4 * buttonSize),
		panelY,
		buttonSize,
		buttonSize * 0.75f
	);

	endButton.setBounds(
		panelX + (5 * buttonSize),
		panelY,
		buttonSize,
		buttonSize * 0.75f
	);

	auto sliderWidth = panelWidth / 16;
	auto sliderHeight = buttonSize;

	gainSlider.setBounds(
		panelWidth - sliderWidth,
		panelY,
		sliderWidth,
		sliderHeight
	);
}

void SongControlPanel::disableButtons(String state)
{
	if(state == "stopped")
	{
		stopButton.setEnabled(false);
		pauseButton.setEnabled(false);
		playButton.setEnabled(true);
	}

	if(state == "playing")
	{
		playButton.setEnabled(false);
		stopButton.setEnabled(true);
		pauseButton.setEnabled(true);
	}

	if(state == "paused")
	{
		pauseButton.setEnabled(false);
		playButton.setEnabled(true);
	}

	if(state == "opened")
	{
		playButton.setEnabled(true);
	}
}
