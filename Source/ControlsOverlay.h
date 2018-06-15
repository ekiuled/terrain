#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Skin.h"
#include "Etc.h"
#include "Inter.h"

class ControlsOverlay
	: public Component
	, public Button::Listener
	, public Slider::Listener
	, public KeyListener{
public:
	ControlsOverlay(DocumentWindow *win, BackgoundColor *bc);
	~ControlsOverlay();
	void resized() override;
	void buttonClicked(Button *button) override;
	void mouseDown(const MouseEvent &event) override;
	void sliderValueChanged(Slider* slider) override;
	void paint(Graphics &g) override;

	bool keyPressed(const KeyPress &key, Component *originatingComponent) override;

	void changeControlsVisible();
	void setInter(Inter *i);
	bool controlsAreVisible();

private:
	SafePointer<DocumentWindow> _window;
	Inter *_shape;
	SafePointer<DrawableButton> _play, _reset;
	Slider _octavesSlider, _widthSlider, _depthSlider, _heightSlider, _persistenceSlider, _rockinessSlider;
	SliderLabel _octavesLabel, _widthLabel, _depthLabel, _heightLabel, _persistenceLabel, _rockinessLabel;
	SliderLabel _colorLabel;
	SafePointer<TextButton> _fly, _static;
	SafePointer<TextButton> _lines, _fill;
	SafePointer<HelpButton> _help;
	ColourChangeButton *_top, *_far, *_near, *_backColor;
	Skin _skin;
	TextButton *_pressedChild;
	bool _controlsVisible;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlsOverlay)
};
