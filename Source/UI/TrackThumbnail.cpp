/*
  ==============================================================================

    TrackThumbnail.cpp
    Created: 2 Aug 2018 3:35:33am
    Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TrackThumbnail.h"

//==============================================================================
TrackThumbnail::TrackThumbnail(
	int sourceSamplesPerThumbnailSamples,
	AudioFormatManager& formatManager,
	AudioThumbnailCache& cache
)
	: thumbnail(sourceSamplesPerThumbnailSamples, formatManager, cache)
{
	thumbnail.addChangeListener(this);
}

TrackThumbnail::~TrackThumbnail()
{
}

void TrackThumbnail::paint (Graphics& g)
{
	if(thumbnail.getNumChannels() == 0)
	{
		paintWithoutFile(g);
	}
	else 
	{
		paintWithFile(g);
	}
}

void TrackThumbnail::resized()
{

}

void TrackThumbnail::setFile(const File& file)
{
	thumbnail.setSource(new FileInputSource(file));
}

void TrackThumbnail::paintWithoutFile(Graphics& g)
{
	g.fillAll(Colours::ghostwhite);

	g.setColour(Colours::black);
	g.drawFittedText("No file loaded.", getLocalBounds(), Justification::centred, 1.0f);
}

void TrackThumbnail::paintWithFile(Graphics& g)
{
	g.fillAll(Colours::ghostwhite);

	g.setColour(Colours::black);
	thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

void TrackThumbnail::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == &thumbnail)
	{
		thumbnailChanged();
	}
}
