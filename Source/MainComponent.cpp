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
	, mainProcessor(new AudioProcessorGraph())
	, audioPlayer(new InternalPlayer(transportSource))
	, pluginManager("Plugins", Colours::ghostwhite)
	, pluginFormatManager(new AudioPluginFormatManager())
{
	for(int i = 0; i < totalPluginsAllowed; ++i)
	{
		thirdPartyPlugins[i] = nullptr;
		builtInPlugins[i] = nullptr;
		slotOccupied[i] = 0;
	}

	addAndMakeVisible(controlPanel);
	addAndMakeVisible(trackUI);
	addAndMakeVisible(pluginPanel);

	addAndMakeVisible(trackThumbnail);
	addAndMakeVisible(positionOverlay);

	controlPanel.addActionListener(this);
	trackUI.addActionListener(this);
	pluginPanel.addActionListener(this);
	pluginManager.addActionListener(this);

	setSize (1600, 900);

	formatManager.registerBasicFormats();
	pluginFormatManager->addFormat(new VSTPluginFormat);
	transportSource.addChangeListener(this);

	mainProcessor->enableAllBuses();
/*
	pluginListWindow->setUsingNativeTitleBar(false);
	pluginListWindow->centreWithSize(400, 200);
*/
/*
	deviceManager.initialiseWithDefaultDevices(2, 2);
	deviceManager.addAudioCallback(&player);
*/
//	player.setProcessor(mainProcessor.get());

	setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
//	pluginListWindow = nullptr;

//	deviceManager.removeAudioCallback(&player);
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	mainProcessor->setPlayConfigDetails(
		2,
		2,
		sampleRate,
		samplesPerBlockExpected
	);
	mainProcessor->prepareToPlay(sampleRate, samplesPerBlockExpected);

	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	initialiseGraph();

//	blockSize = samplesPerBlockExpected;
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if(readerSource.get() == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
	}
	else
	{
//		updateGraph();

		trackUI.sendActionMessage("time");

		if(looping)
		{
			positionOverlay.getLoopBounds(loopStartTime, loopEndTime);

//			DBG(loopEndTime);
			if(loopEndTime > 0.0 && transportSource.getCurrentPosition() > loopEndTime)
			{
				transportSource.setPosition(loopStartTime);
			}
		}

		mainProcessor->processBlock(*(bufferToFill.buffer), MidiBuffer());
	}
}

void MainComponent::releaseResources()
{
/*	
	if(!pluginNodes.isEmpty())
	{
		pluginNodes.clear();
	}
	if(!pluginNodeEditors.isEmpty())
	{
		pluginNodeEditors.clear();
	}
*/
//	pluginFormatManager.release();

	for (auto node : mainProcessor->getNodes())
	{
		mainProcessor->removeNode(node->nodeID);
	}

	mainProcessor->releaseResources();
	audioPlayer.release();
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

	trackUI.setBounds(
		padding,
		2 * padding,
		getWidth() - (2 * padding),
		getHeight() / 2
	);

	pluginPanel.setBounds(
		(getWidth() - (getWidth() * 0.40)) - padding,
		getHeight() * 0.55f,
		getWidth() * 0.40,
		getHeight() * 0.45f - padding
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
		int position = (int)(transportSource.getCurrentPosition());

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

		trackUI.changeTime(timeDisplay);
	}
	if(message == "loop")
	{
		loopButtonClicked();
	}
	if(message == "gain")
	{
		float gainInDb = controlPanel.getGain();
		float gain = powf(10.0f, gainInDb * 0.05f);

		transportSource.setGain(gain);
	}
	if(message.startsWith("FX"))
	{
		loadPlugin(message);
	}
	if(message == "pluginload")
	{
		if(pluginManager.pluginHasBeenSelected)
		{
			PluginDescription pluginDesc = pluginManager.getSelectedPlugin();
			
			pluginManager.pluginHasBeenSelected = false;

			createNode(pluginDesc);
		}
	}
}


void MainComponent::changeListenerCallback(ChangeBroadcaster * source)
{

}

void MainComponent::createNode(PluginDescription desc)
{
	AudioPluginInstance* instance;
//	AudioProcessorEditor* editor;

	String error;
	instance = pluginFormatManager->createPluginInstance(desc, 44100, 441, error);

//	editor = instance->createEditor();

	thirdPartyPlugins[slotNumber] = mainProcessor->addNode(instance);
/*
	pluginNodes.add(&newNode);
	pluginNodeEditors.add(editor);
*/

	updateGraph();
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
	/*
	for(auto i = buffers.size(); --i >= 0;)
	{
		ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i));

		if(buffer->getReferenceCount() == 2)
		{
			buffers.remove(i);
		}
	}
	*/
}

void MainComponent::initialiseGraph()
{
	mainProcessor->clear();

	audioInputNode = mainProcessor->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::audioInputNode));
	audioOutputNode = mainProcessor->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::audioOutputNode));
	audioPlaybackNode = mainProcessor->addNode(audioPlayer.get());

	connectAudioNodes();
}

void MainComponent::connectAudioNodes()
{
	for(int channel = 0; channel < 2; ++channel)
	{
		mainProcessor->addConnection({
			{ audioInputNode->nodeID, channel },
			{ audioPlaybackNode->nodeID, channel }
		});

		mainProcessor->addConnection({
			{audioPlaybackNode->nodeID, channel},
			{audioOutputNode->nodeID, channel}
		});
	}

	for (auto node : mainProcessor->getNodes())
	{
		node->getProcessor()->enableAllBuses();
	}

}

void MainComponent::updateGraph()
{
	ReferenceCountedArray<Node> activeNodes; 

	for (int i = 0; i < totalPluginsAllowed; ++i)
	{
		if (thirdPartyPlugins[i] != nullptr)
		{
			activeNodes.add(thirdPartyPlugins[i]);
		}
		if(builtInPlugins[i] != nullptr)
		{
			activeNodes.add(builtInPlugins[i]);
		}
	}

	// remove and add some connections
	for(int channel = 0; channel < 2; ++channel)
	{
		if(!graphHasPlugins)
		{
			// this should only happen when there is one unconnected plugin in the activeNodes array
			// so using the getFirst() function might work, but i guess i'll find out soon enough

			mainProcessor->removeConnection({
				{ audioPlaybackNode->nodeID, channel },
				{ audioOutputNode->nodeID, channel }
			});

			mainProcessor->addConnection({
				{ audioPlaybackNode->nodeID, channel },
				{ activeNodes.getFirst()->nodeID, channel }
			});

			mainProcessor->addConnection({
				{ activeNodes.getFirst()->nodeID, channel },
				{ audioOutputNode->nodeID, channel }
			});
		}
		else
		{
			for(int i = 0; i < activeNodes.size() - 1; ++i)
			{
				if(!(mainProcessor->isConnected(activeNodes[i]->nodeID, activeNodes[i + 1]->nodeID)))
				{
					if(mainProcessor->isConnected(activeNodes[i]->nodeID, audioOutputNode->nodeID))
					{
						mainProcessor->removeConnection({
							{activeNodes[i]->nodeID, channel},
							{audioOutputNode->nodeID, channel}
						});

						mainProcessor->addConnection({
							{ activeNodes[i]->nodeID, channel },
							{ activeNodes[i + 1]->nodeID, channel }
						});

						mainProcessor->addConnection({
							{ activeNodes[i + 1]->nodeID, channel },
							{ audioOutputNode->nodeID, channel }
						});
					}
					else if(mainProcessor->isConnected(activeNodes[i + i]->nodeID, audioOutputNode->nodeID))
					{
						if(mainProcessor->isConnected(audioPlaybackNode->nodeID, activeNodes[i + 1]->nodeID))
						{
							mainProcessor->removeConnection({
								{audioPlaybackNode->nodeID, channel},
								{activeNodes[i + 1]->nodeID, channel}
							});

							mainProcessor->addConnection({
								{audioPlaybackNode->nodeID, channel},
								{activeNodes[i]->nodeID, channel}
							});

							mainProcessor->addConnection({
								{ activeNodes[i]->nodeID, channel },
								{ activeNodes[i + 1]->nodeID, channel }
							});
						}
					}
				}
			}
		}
	}

	for (auto node : mainProcessor->getNodes())
	{
		node->getProcessor()->enableAllBuses();
	}

	graphHasPlugins = true;
}

void MainComponent::loadPlugin(String fxCode)
{
	slotNumber = fxCode[2] - '0';

	if(fxCode.endsWithChar('5'))
	{
		
	}

	if(fxCode.endsWithChar('1'))
	{
		// 3rd party plugin requested
		pluginManager.setVisible(true);
		if(!pluginDirScanned)
		{
			if(pluginManager.addAndScanDirectory())
			{
				pluginManager.update();

				pluginDirScanned = true;
			}
			else 
			{
				AlertWindow::showMessageBox(
					AlertWindow::WarningIcon,
					"No Plugins Found",
					"No plugins were found.\nPlease scan another directory.",
					"Ok"
				);
			}
		}
	}
	else
	{
		// load built in plugin
		slotOccupied[slotNumber] = true;
	}
}
