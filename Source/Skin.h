#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Inter.h"

class Skin
	: public LookAndFeel_V3 {
public:
	Skin();
	~Skin();

	void positionDocumentWindowButtons(DocumentWindow&,
		int titleBarX, int titleBarY,
		int titleBarW, int titleBarH,
		Button* minimiseButton,
		Button* maximiseButton,
		Button* closeButton,
		bool positionTitleBarButtonsOnLeft) override;
	Button* createDocumentWindowButton(int buttonType) override;
	void drawDocumentWindowTitleBar(DocumentWindow &window, Graphics &g, int w, int h, int titleSpaceX, int titleSpaceW, const Image *icon, bool drawTitleTextOnLeft) override;

	void drawLabel(Graphics &g, Label &l) override;
	void drawPlayButton(DrawableButton &b);
	void drawResetButton(DrawableButton &b);
	void drawDrawableButton(Graphics &g, DrawableButton &b, bool isMouseOverButton, bool isButtonDown) override;

	void drawLinearSliderBackground(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) override;
	void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

	void drawButtonBackground(Graphics &g, Button &b, const Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown) override;
	void drawButtonText(Graphics &g, TextButton &b, bool isMouseOverButton, bool isButtonDown) override;

private:
	class CircleWindowButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Skin)
};

class Skin::CircleWindowButton : public Button {
public:
	CircleWindowButton(const String& name, Colour col, Colour out, Colour pathcol,
		const Path& normalShape_,
		const Path& toggledShape_) noexcept
		: Button(name),
		colour(col),
		outline(out),
		main(pathcol),
		normalShape(normalShape_),
		toggledShape(toggledShape_) {
	}

	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
	Colour colour, outline, main;
	Path normalShape, toggledShape;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircleWindowButton)
};
