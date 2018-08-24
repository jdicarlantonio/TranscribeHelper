/*
  ==============================================================================

    PluginManager.h
    Created: 21 Aug 2018 1:46:59pm
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

class PluginManager
	: public DocumentWindow
	, public ListBoxModel
	, public ActionBroadcaster
{
public:
	PluginManager(const String& name, Colour backgroundColour);
	~PluginManager();

	void resized() override;

	int getNumRows() override { return numPlugins; }
	void paintListBoxItem(
		int rowNumber,
		Graphics& g,
		int width,
		int height,
		bool rowIsSelected
	) override;
	void listBoxItemDoubleClicked(int row, const MouseEvent& ev) override;

	void closeButtonPressed() override
	{
		setVisible(false);
	}
	
	bool addAndScanDirectory();
	void update() { pluginSelector.updateContent(); }
	PluginDescription getSelectedPlugin();

public:
	bool pluginHasBeenSelected { false };

private:
	TextButton changeVstDirButton;
	TextButton scanVstDirButton;
	ListBox pluginSelector { {}, this };
	int numPlugins { 0 };

	String pluginDirectoryPath;

	std::unique_ptr<KnownPluginList> pluginList;
	std::unique_ptr<AudioPluginFormat> formatManager;

//	OwnedArray<PluginDescription> descriptions;

	std::unique_ptr<KnownPluginList::PluginTree> pluginTree;
	PluginDescription selectedPlugin;

//	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginManager)
};