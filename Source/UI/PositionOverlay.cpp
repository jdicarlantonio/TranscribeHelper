/*
  ==============================================================================

    PositionOverlay.cpp
    Created: 3 Aug 2018 5:29:02am
    Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "PositionOverlay.h"

//==============================================================================
PositionOverlay::PositionOverlay(AudioTransportSource& transportSourceToUse)
	: transportSource(transportSourceToUse)
	, looping(false)
{
	startTimer(40);
}

  PositionOverlay::~PositionOverlay()
{
}

void PositionOverlay::paint (Graphics& g)
{
	if(loopStartTime > 0.0 && loopEndTime > 0.0)
	{
		drawLoopOverlays(g);
	}
	
	auto duration = transportSource.getLengthInSeconds();

	if(duration > 0.0)
	{
		auto audioPosition = transportSource.getCurrentPosition();	
		auto drawPosition = (audioPosition / duration) * getWidth();

		g.setColour(Colours::indianred);
		g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight() - 2.0f, 4.0f);
	}

}

void PositionOverlay::resized()
{

}

void PositionOverlay::mouseDown(const MouseEvent& event)
{
	auto duration = transportSource.getLengthInSeconds();

	if(duration > 0.0)
	{
		auto clickPosition = event.position.x;
		auto audioPosition = (clickPosition / getWidth()) * duration;

		transportSource.setPosition(audioPosition);
	}
}

void PositionOverlay::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{

	if(wheel.deltaY < 0.0)
	{
		// to avoid drawing a start time bigger than end time
		auto previousStartTime = loopStartTime;
		auto previousStartDraw = loopStartDrawPos;

		auto duration = transportSource.getLengthInSeconds();

		if (duration > 0.0)
		{
			auto clickPosition = event.position.x;
			auto audioPosition = (clickPosition / getWidth()) * duration;

			loopStartDrawPos = (audioPosition / duration) * getWidth();
			loopStartTime = audioPosition;

			if(loopEndTime > 0.0 && loopStartTime > loopEndTime)
			{
				loopStartTime = previousStartTime;
				loopStartDrawPos = previousStartDraw;
			}
		}
	}
	else if(wheel.deltaY > 0.0)
	{
		auto previousEndTime = loopEndTime;
		auto previousEndDraw = loopEndDrawPos;

		auto duration = transportSource.getLengthInSeconds();

		if (duration > 0.0)
		{
			auto clickPosition = event.position.x;
			auto audioPosition = (clickPosition / getWidth()) * duration;

			loopEndDrawPos = (audioPosition / duration) * getWidth();
			loopEndTime = audioPosition;

			if (loopStartTime > 0.0 && loopStartTime > loopEndTime)
			{
				loopEndTime = previousEndTime;
				loopEndDrawPos = previousEndDraw;
			}
		}
	}
	
	repaint();
}

void PositionOverlay::drawLoopOverlays(Graphics& g)
{
	g.setColour(Colours::green);
	g.drawLine(loopStartDrawPos, 0.0f, loopStartDrawPos, (float)getHeight() - 2.0f, 4.0f);

	g.setColour(Colours::darkred);
	g.drawLine(loopEndDrawPos, 0.0f, loopEndDrawPos, (float)getHeight() - 2.0f, 4.0f);
}

void PositionOverlay::getLoopBounds(double& start, double& end)
{
	start = loopStartTime;
	end = loopEndTime;
}

