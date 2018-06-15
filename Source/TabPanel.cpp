#include "TabPanel.h"

TabPanel::TabPanel()
	: TabbedComponent(TabbedButtonBar::TabsAtTop) {
	_button.setButtonText("button");
	_button.setBounds(100, 0, 100, 100);
	addAndMakeVisible(_button);
	addTab("Playground", Colours::lightpink, _playground = new PlaygroundComponent, true);
	addTab("Options", Colours::lightgoldenrodyellow, _options = new OptionsComponent, true);
}

TabPanel::~TabPanel() {
	//removeAllChildren();
}
