/*
  ==============================================================================

    PluginManager.cpp
    Created: 21 Aug 2018 1:46:59pm
    Author:  jd

  ==============================================================================
*/

#include "PluginManager.h"

PluginManager::PluginManager(const String& name, Colour backgroundColour)
	: DocumentWindow(name, backgroundColour, DocumentWindow::closeButton)
	, pluginList(new KnownPluginList())
	, formatManager(new VSTPluginFormat())
	, pluginTree(new KnownPluginList::PluginTree)
{
	Component::addAndMakeVisible(changeVstDirButton);
	changeVstDirButton.setButtonText("Change Plugin Directory");
	changeVstDirButton.setColour(TextButton::buttonColourId, Colours::black);

	Component::addAndMakeVisible(scanVstDirButton);
	scanVstDirButton.setButtonText("Scan Directory");
	scanVstDirButton.setColour(TextButton::buttonColourId, Colours::black);

	Component::addAndMakeVisible(pluginSelector);
	pluginSelector.setColour(ListBox::backgroundColourId, Colours::black);
	pluginSelector.setColour(ListBox::outlineColourId, Colours::ghostwhite);
	pluginSelector.setOutlineThickness(1);

//	descriptions = OwnedArray<PluginDescription>();


	setUsingNativeTitleBar(true);
	centreWithSize(400, 200);
}

PluginManager::~PluginManager()
{
//	pluginTree.release();
}

void PluginManager::resized()
{
	float x = getLocalBounds().getX();
	float y = getLocalBounds().getY();
	float width = getLocalBounds().getWidth();
	float height = getLocalBounds().getHeight();

	float buttonHeight = height / 8.0f;

	changeVstDirButton.setBounds(
		x,
		y,
		width * 0.5f,
		buttonHeight
	);

	scanVstDirButton.setBounds(
		width * 0.5f,
		y,
		width * 0.5f,
		buttonHeight
	);

	pluginSelector.setBounds(
		x,
		y + buttonHeight,
		width,
		height - buttonHeight
	);
}

void PluginManager::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
	if(numPlugins >= 1)
	{
		if (rowIsSelected)
		{
			g.fillAll(Colours::lightblue);
		}

		String pluginName = pluginTree->plugins[rowNumber]->name;

		g.setColour(Colours::white);
		g.setFont(height * 0.7f);

		g.drawText(
			pluginName,
			5, 
			0, 
			width, 
			height,
			Justification::centredLeft
		);
	}
}

void PluginManager::listBoxItemDoubleClicked(int row, const MouseEvent& ev)
{
	selectedPlugin = *(pluginTree->plugins[row]);
	pluginHasBeenSelected = true;

	sendActionMessage("pluginload");

	setVisible(false);
}

bool PluginManager::addAndScanDirectory()
{
	FileSearchPath vstDirectory;

	FileChooser chooser(
		"Choose Directory",
		File::getSpecialLocation(File::userHomeDirectory),
		String(),
		true
	);

	File directory;
	if(chooser.browseForMultipleFilesOrDirectories())
	{
		directory = chooser.getResult();

		pluginDirectoryPath = directory.getFullPathName();
	}

	vstDirectory.add(directory);

	PluginDirectoryScanner pluginScanner(
		*pluginList.get(),
		*formatManager.get(),
		vstDirectory,
		false,
		File()
	);

	String pluginName;

	bool scanning = true;
	while(scanning)
	{
		scanning = pluginScanner.scanNextFile(true, pluginName);
	}

	pluginTree.reset(pluginList->createTree(KnownPluginList::sortAlphabetically));

	numPlugins = pluginTree->plugins.size();

	return (numPlugins >= 1); // at least one plugin was loaded
}

PluginDescription PluginManager::getSelectedPlugin()
{
	return selectedPlugin;
}
