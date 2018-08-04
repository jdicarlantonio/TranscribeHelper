/*
  ==============================================================================

    TrackThumbnail.h
    Created: 2 Aug 2018 3:35:33am
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class TrackThumbnail    
	: public Component
	, public ChangeListener
{
public:
    TrackThumbnail(
		int sourceSamplesPerThumbnailSamples,
		AudioFormatManager& formatManager,
		AudioThumbnailCache& cache
	);
    ~TrackThumbnail();

    void paint (Graphics&) override;
    void resized() override;

	void setFile(const File& file);
	void paintWithoutFile(Graphics& g); // no file loaded
	void paintWithFile(Graphics& g);

	void changeListenerCallback(ChangeBroadcaster* source) override;

private:
	void thumbnailChanged() { repaint(); }

	AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackThumbnail)
};
