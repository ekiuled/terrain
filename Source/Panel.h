#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OptionsComponent.h"
#include "PlaygroundComponent.h"

class Panel
	: public MultiDocumentPanel {
public:
	Panel();
	Panel(int w, int h);
	~Panel();

	bool tryToCloseDocument(Component *c) override;
	void paint(Graphics &g) override;

private:
	ScopedPointer<OptionsComponent> _options;
	ScopedPointer<PlaygroundComponent> _playground;
};