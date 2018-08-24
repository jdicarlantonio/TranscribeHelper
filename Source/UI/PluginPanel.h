/*
  ==============================================================================

    PluginPanel.h
    Created: 16 Aug 2018 12:54:19pm
    Author:  jd

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

static constexpr int totalPluginsAllowed = 5;

class PluginPanel    
	: public Component
	, public ActionBroadcaster
{
public:
    PluginPanel();
    ~PluginPanel();

    void paint (Graphics&) override;
    void resized() override;

private:
	std::array<ComboBox, totalPluginsAllowed> pluginSlots;
	std::array<Label, totalPluginsAllowed> slotLabels;

	// currently only one choice while i work on some built in effects
	// those being time stretch, pitch shift, and EQ
	StringArray choices = {
		"Add 3rd Party...", 
		"Equalizer",
		"Time Stretch",
		"Pitch Shift",
		"None"
	};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginPanel)
};
