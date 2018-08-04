/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
	: state(STOPPED)
	, thumbnailCache(5)
	, trackThumbnail(512, formatManager, thumbnailCache)
	, positionOverlay(transportSource)
	, loopStartTime(0.0)
	, loopEndTime(0.0)
	, looping(false)
{
	addAndMakeVisible(controlPanel);
	addAndMakeVisible(dspControl);
	addAndMakeVisible(trackUI);

	addAndMakeVisible(trackThumbnail);
	addAndMakeVisible(positionOverlay);

	controlPanel.addActionListener(this);
	trackUI.addActionListener(this);

	setSize (1600, 900);

	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);

	setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if(readerSource.get() == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
	}
	else
	{
		int position = (int)(transportSource.getCurrentPosition());

		timeDisplay;
		if (position < 10)
		{
			timeDisplay = "00:0";
			timeDisplay.append(String(position), 1);
		}
		if (position >= 10 && position < 60)
		{
			timeDisplay = "00:";
			timeDisplay.append(String(position), 2);
		}
		if (position >= 60)
		{
			timeDisplay = "0";
			int minute = position / 60;
			int second = position - (minute * 60);

			timeDisplay.append(String(minute), 1);
			timeDisplay += ':';

			if (second < 10)
			{
				timeDisplay += '0';
				timeDisplay.append(String(second), 1);
			}
			else
			{
				timeDisplay.append(String(second), 2);
			}
		}

		trackUI.sendActionMessage("time");

		if(looping)
		{
			positionOverlay.getLoopBounds(loopStartTime, loopEndTime);

//			DBG(loopEndTime);
			if(loopEndTime > 0.0 && transportSource.getCurrentPosition() > loopEndTime)
			{
				transportSource.setPosition(loopStartTime);
			}

			transportSource.getNextAudioBlock(bufferToFill);
		}
		else
		{
			transportSource.getNextAudioBlock(bufferToFill);
		}
	}
}

void MainComponent::releaseResources()
{
	transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
	g.fillAll (
		/*getLookAndFeel().findColour (ResizableWindow::backgroundColourId)*/
		Colours::ghostwhite
	);
}

void MainComponent::resized()
{
	int padding = 10;

	controlPanel.setBounds(
		padding,
		getHeight() * 0.85f,
		getWidth() * 0.50f,
		(getHeight() * 0.15f) - padding
	);

	dspControl.setBounds(
		controlPanel.getWidth() + (2 * padding),
		getHeight() * 0.60f,
		getWidth() * 0.50f - (4 * padding),
		(getHeight() * 0.40f) - padding
	);

	trackUI.setBounds(
		padding,
		2 * padding,
		getWidth() - (2 * padding),
		getHeight() / 2
	);

	int trackX = trackUI.getX();
	int trackY = trackUI.getY();
	int trackWidth = trackUI.getWidth();
	int trackHeight = trackUI.getHeight();
	Rectangle<int> thumbnailBounds(
		trackX,
		trackHeight * 0.42,
		trackWidth,
		trackHeight / 4
	);

	trackThumbnail.setBounds(thumbnailBounds);
	positionOverlay.setBounds(thumbnailBounds);
}

void MainComponent::openButtonClicked()
{
//	DBG("open clicked");
	FileChooser chooser(
		"Choose File",
		File::getSpecialLocation(File::userHomeDirectory),
		"*.wav; *.mp3; *.flac"
	);

	if(chooser.browseForFileToOpen())
	{
		File file = chooser.getResult();

		if(auto* reader = formatManager.createReaderFor(file))
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

			trackThumbnail.setFile(file);
			trackUI.changeOpenButtonText(file.getFileName());
			readerSource.reset(newSource.release());

			controlPanel.disableButtons("opened");
		}
	}
}

void MainComponent::playButtonClicked()
{
	changeState(PLAYING);
}

void MainComponent::stopButtonClicked()
{
	changeState(STOPPED);
}

void MainComponent::pauseButtonClicked()
{
	changeState(PAUSED);
}

void MainComponent::loopButtonClicked()
{
	if(trackUI.isLooping())
	{
//		DBG("True");
		positionOverlay.setLooping(true);
		looping = true;
	}
	else
	{
//		DBG("False");
		positionOverlay.setLooping(false);
		looping = false;
	}

}

void MainComponent::actionListenerCallback(const String& message)
{
	if(message == "open")
	{
		openButtonClicked();
	}
	if (message == "play")
	{
		playButtonClicked();
	}
	if (message == "stop")
	{
		stopButtonClicked();
	}
	if (message == "pause")
	{
		pauseButtonClicked();
	}
	if(message == "time")
	{
		trackUI.changeTime(timeDisplay);
	}
	if(message == "loop")
	{
		loopButtonClicked();
	}
}


void MainComponent::changeListenerCallback(ChangeBroadcaster * source)
{
	// Nothing to do here yet, probably because I'm handling playing audio files in
	// a stupid way
}

void MainComponent::changeState(TrackState state)
{
	switch(state)
	{
		case STOPPED:
		{
			transportSource.stop();

			if(!looping)
			{
				transportSource.setPosition(0.0);
			}
			else
			{
				transportSource.setPosition(loopStartTime);
			}

			controlPanel.disableButtons("stopped");

			break;
		}
		case PLAYING:
		{
			transportSource.start();
			controlPanel.disableButtons("playing");

			break;
		}
		case PAUSED:
		{
			transportSource.stop();
			controlPanel.disableButtons("paused");

			break;
		}
	}
}

/*
void MainComponent::transportSourceChanged()
{

}
*/

void MainComponent::checkForBuffersToFree()
{
	for(auto i = buffers.size(); --i >= 0;)
	{
		ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i));

		if(buffer->getReferenceCount() == 2)
		{
			buffers.remove(i);
		}
	}
}
