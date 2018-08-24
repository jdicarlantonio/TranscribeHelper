/*
  ==============================================================================

    PluginPanel.cpp
    Created: 16 Aug 2018 12:54:19pm
    Author:  jd

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "PluginPanel.h"

//==============================================================================
PluginPanel::PluginPanel()
{
	for(int i = 0; i < totalPluginsAllowed; ++i)
	{
		addAndMakeVisible(pluginSlots[i]);
		pluginSlots[i].setColour(ComboBox::backgroundColourId, Colours::black);
		pluginSlots[i].setColour(ComboBox::textColourId, Colours::ghostwhite);
		pluginSlots[i].addItemList(choices, 1);

		addAndMakeVisible(slotLabels[i]);
		String slotString = "FX ";
		slotString.append(String(i), 1);
		slotLabels[i].setText(slotString, dontSendNotification);
		slotLabels[i].setColour(Label::textColourId, Colours::black);

		slotLabels[i].attachToComponent(&pluginSlots[i], true);

		pluginSlots[i].onChange = [this, i] {
			String fxCode = "FX";
			fxCode.append(String(i), 1);
			fxCode.append(String(pluginSlots[i].getSelectedId()), 1);
			
			DBG("Hey");

			sendActionMessage(fxCode);
		};
	}
}

PluginPanel::~PluginPanel()
{
}

void PluginPanel::paint (Graphics& g)
{
//	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

//    g.setColour (Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PluginPanel::resized()
{
	FlexBox fb;
	fb.flexDirection = FlexBox::Direction::column;
	fb.justifyContent = FlexBox::JustifyContent::center;
	fb.alignContent = FlexBox::AlignContent::center;

	auto width = getLocalBounds().getWidth() * 0.75f;
	auto height = getLocalBounds().getHeight() / 7.0f;

	Array<FlexItem> slots;
	for(int i = 0; i < totalPluginsAllowed; ++i)
	{
		slots.add(FlexItem(width, height, pluginSlots[i]));
	}

	fb.items.addArray(slots);
	fb.performLayout(getLocalBounds().toFloat());
}
