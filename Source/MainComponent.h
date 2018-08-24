/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// UI components
#include "UI/SongControlPanel.h"
#include "UI/TrackUI.h"
#include "UI/PositionOverlay.h"
#include "UI/PluginPanel.h"

// built in effects
#include "FX/InternalPlayer.h"

// for plugin support
#include "FX/PluginManager.h"

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
	using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
	using Node = AudioProcessorGraph::Node;

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

	void createNode(PluginDescription desc);

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

	// processor graph stuff
	void initialiseGraph();
	void connectAudioNodes();
	void updateGraph();
	void loadPlugin(String fxCode);

private:
	// store an array and single instance of a reference counted audio buffer
//	ReferenceCountedArray<ReferenceCountedBuffer> buffers; // buffers needed by audio thread
//	ReferenceCountedBuffer::Ptr currentBuffer;

	String chosenPath;

	SongControlPanel controlPanel;
	TrackUI trackUI;

	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;

	TrackState state;

	AudioTransportSource transportSource;

	AudioThumbnailCache thumbnailCache;
	TrackThumbnail trackThumbnail;
	PositionOverlay positionOverlay;
//	Slider zoomSlider; TODO
	String timeDisplay;

	bool looping;
	double loopStartTime;
	double loopEndTime;

	int blockSize;

	std::unique_ptr<AudioPluginFormatManager> pluginFormatManager;
	PluginManager pluginManager;
	bool pluginDirScanned { false };

	std::unique_ptr<AudioProcessorGraph> mainProcessor;
	bool graphHasPlugins { false };

	// internal plugin for audio file playback
	std::unique_ptr<InternalPlayer> audioPlayer;

//	AudioProcessorPlayer player;

	Node::Ptr audioInputNode;
	Node::Ptr audioPlaybackNode;
	Node::Ptr audioOutputNode;

	// should keep track of which slot occupies a third party plugin
	Node::Ptr thirdPartyPlugins[totalPluginsAllowed]; 
	Node::Ptr builtInPlugins[totalPluginsAllowed];

	// should indicate which slot is a built in effect
	bool slotOccupied[totalPluginsAllowed];

	Node::Ptr currentLoadedPlugin;
//	OwnedArray<Node::Ptr> pluginNodes;
//	OwnedArray<AudioProcessorEditor> pluginNodeEditors;
	
	Node::Ptr midiInputNode;
	Node::Ptr midiOutputNode;

	// plugin UI
	PluginPanel pluginPanel;
	int slotNumber;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
