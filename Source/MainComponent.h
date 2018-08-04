/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// user includes
#include "UI/SongControlPanel.h"
#include "UI/DSPControlPanel.h"
#include "UI/TrackUI.h"
#include "UI/PositionOverlay.h"
#include "ReferenceCountedBuffer.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent   
	: public AudioAppComponent
	, public ActionListener
	, public ChangeListener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint (Graphics& g) override;
	void resized() override;

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void pauseButtonClicked();
	void loopButtonClicked();

	void actionListenerCallback(const String& message) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;


private:
	enum TrackState
	{
		STOPPED,
		PLAYING,
		PAUSED
	};
	void changeState(TrackState state);
//	void transportSourceChanged();

	void checkForBuffersToFree();

private:
	// store an array and single instance of a reference counted audio buffer
	ReferenceCountedArray<ReferenceCountedBuffer> buffers; // buffers needed by audio thread
	ReferenceCountedBuffer::Ptr currentBuffer;

	String chosenPath;

	SongControlPanel controlPanel;
	DSPControlPanel dspControl;
	TrackUI trackUI;

	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;

	TrackState state;


	AudioTransportSource transportSource;

	AudioThumbnailCache thumbnailCache;
	TrackThumbnail trackThumbnail;
	PositionOverlay positionOverlay;
	String timeDisplay;

	bool looping;
	double loopStartTime;
	double loopEndTime;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
