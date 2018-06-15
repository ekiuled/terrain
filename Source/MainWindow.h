#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Inter.h"
#include "ContentComponent.h"

class MainWindow
	: public DocumentWindow
	, public Button::Listener{
public:
	MainWindow();
	~MainWindow();
	void closeButtonPressed() override;
	void resized() override;
	void buttonClicked(Button *button) override;

private:
	SafePointer<ContentComponent> _contentComponent;
	SafePointer<DrawableButton> _menu;
	Skin _skin;
	//SystemTrayIconComponent _trayIcon;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
