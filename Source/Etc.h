#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Inter.h"

class SliderLabel
	: public Label {
	void paint(Graphics &g) override;
};

class HelpButton
	: public TextButton {
public:
	HelpButton();
	void paint(Graphics &g) override;

private:
	int _n, _m;
	const char *_keys;
	const char *_text;
	const char *_mouseStuff;
};

class ColourChangeButton
	: public TextButton
	, public ChangeListener 
{
public:
	ColourChangeButton(const String &s, Options option, Colour c);
	void clicked() override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void setInter(Inter *i);
	void setBackgroundColorCC(BackgoundColor *bc);
	void kill();

	Colour getColor();

private:
	Inter *_shape;
	BackgoundColor *_cc;
	Colour _currColor;
	Options _opt;
};

class ColorSelectorAndBroadcaster
	: public ColourSelector{
public:
	ColorSelectorAndBroadcaster(int flags, ColourChangeButton *button);
	~ColorSelectorAndBroadcaster();

private:
	ColourChangeButton *_parent;
};