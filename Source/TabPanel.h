#pragma once
#include  "../JuceLibraryCode/JuceHeader.h"
#include "OptionsComponent.h"
#include "PlaygroundComponent.h"

class TabPanel
	: public TabbedComponent {
public:
	TabPanel();
	~TabPanel();

private:
	TextButton _button;
	ScopedPointer<OptionsComponent> _options;
	ScopedPointer<PlaygroundComponent> _playground;
};
