#include "Skin.h"

Skin::Skin() {
}

Skin::~Skin() {
}


void Skin::positionDocumentWindowButtons(DocumentWindow &, int titleBarX, int titleBarY, int titleBarW, int titleBarH, Button * minimiseButton, Button * maximiseButton, Button * closeButton, bool positionTitleBarButtonsOnLeft) {
	const int buttonW = titleBarH - titleBarH / 8;

	int x = positionTitleBarButtonsOnLeft ? titleBarX + 4
		: titleBarX + titleBarW - buttonW - buttonW / 4;

	if (closeButton != nullptr) {
		closeButton->setBounds(x, titleBarY, buttonW, titleBarH);
		x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
	}

	if (positionTitleBarButtonsOnLeft)
		std::swap(minimiseButton, maximiseButton);

	if (maximiseButton != nullptr) {
		maximiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
		x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
	}

	if (minimiseButton != nullptr)
		minimiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
}

Button *Skin::createDocumentWindowButton(int buttonType) {
	Path shape;
	const float crossThickness = 0.2f;

	if (buttonType == DocumentWindow::closeButton) {
		shape.addLineSegment(Line<float>(0.0f, 0.0f, 1.0f, 1.0f), crossThickness);
		shape.addLineSegment(Line<float>(1.0f, 0.0f, 0.0f, 1.0f), crossThickness);

		return new CircleWindowButton("close", Colour(0xffff5749), Colour(0xff7e2d27), Colour(0xff7e2d27), shape, shape);
	}

	if (buttonType == DocumentWindow::minimiseButton) {
		shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

		return new CircleWindowButton("minimise", Colour(0xfff9da6e), Colour(0xffeea923), Colour(0xff9e6b34), shape, shape);
	}

	if (buttonType == DocumentWindow::maximiseButton) {
		shape.addLineSegment(Line<float>(0.5f, 0.0f, 0.5f, 1.0f), crossThickness);
		shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

		Path fullscreenShape;
		fullscreenShape.addTriangle(0.3f, 0.0f, 1.0f, 0.7f, 1.0f, 0.0f);
		fullscreenShape.addTriangle(0.0f, 0.3f, 0.0f, 1.0f, 0.7f, 1.0f);

		return new CircleWindowButton("maximise", Colour(0xff5fce5f), Colour(0xff327832), Colour(0xff327832), shape, fullscreenShape);
	}

	jassertfalse;
	return nullptr;
}

void Skin::drawDocumentWindowTitleBar(DocumentWindow &window, Graphics &g, int w, int h, int titleSpaceX, int titleSpaceW, const Image * icon, bool drawTitleTextOnLeft) {
	if (w * h == 0)
		return;

	const bool isActive = window.isActiveWindow();

	g.setColour(Colour(0xff2d2d30));
	g.fillAll();

	Font font(h * 0.65f, Font::bold);
	g.setFont(font);

	int textW = font.getStringWidth(window.getName());
	int iconW = 0;
	int iconH = 0;

	if (icon != nullptr) {
		iconH = (int)font.getHeight();
		iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
	}

	textW = jmin(titleSpaceW, textW + iconW);
	int textX = drawTitleTextOnLeft ? titleSpaceX
		: jmax(titleSpaceX, (w - textW) / 2);

	if (textX + textW > titleSpaceX + titleSpaceW)
		textX = titleSpaceX + titleSpaceW - textW;

	if (icon != nullptr) {
		g.setOpacity(isActive ? 1.0f : 0.6f);
		g.drawImageWithin(*icon, textX, (h - iconH) / 2, iconW, iconH,
			RectanglePlacement::centred, false);
		textX += iconW + 8;
		textW -= iconW;
	}

	if (window.isColourSpecified(DocumentWindow::textColourId) || isColourSpecified(DocumentWindow::textColourId))
		g.setColour(window.findColour(DocumentWindow::textColourId));
	else
		g.setColour(window.getBackgroundColour().contrasting(isActive ? 0.7f : 0.4f));

	g.setFont(Font(iconH, Font::plain));
	g.drawText(window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}

void Skin::drawLabel(Graphics & g, Label & l) {
	Rectangle<int> area = l.getBounds();
	float w = l.getWidth(), h = l.getHeight(), off = 3.0f;
	g.setColour(Colour::fromRGBA(255, 255, 255, 200));
	g.fillRect(off, off, w - 2 * off, h - 2 * off);
	g.setColour(Colour::fromRGB(150, 150, 150));
	g.drawRect(off, off, w - 2 * off, h - 2 * off, 1.0f);

	g.setColour(Colour::fromRGB(0, 0, 0));
	g.drawText(l.getText(), area, Justification::horizontallyCentred);
}

void Skin::drawPlayButton(DrawableButton &b) {
	DrawablePath normal, over, down;
	Path t;
	float w = 15.0f, h = 15.0f;
	float cx = w * 0.5f, cy = h * 0.5f;
	float r = cx * 0.5f;
	t.addTriangle(cx - r, cy - r, cx - r, cy + r, cx + r, cy);

	normal.setPath(t);
	over.setPath(t);
	down.setPath(t);

	normal.setFill(Colour::fromRGBA(199, 252, 223, 150)); //203, 240, 187
	over.setFill(Colour::fromRGBA(199, 252, 223, 240));
	down.setFill(Colour::fromRGBA(199, 252, 223, 255));

	b.setImages(&normal, &over, &down);
}

void Skin::drawResetButton(DrawableButton & b) {
	DrawablePath normal, over, down;
	Path p;
	float w = 15.0f, h = 15.0f;
	p.startNewSubPath(w, h * 0.5);
	p.quadraticTo(w * 0.8, h, w * 0.3, h * 0.9);
	p.lineTo(w * 0.4, h * 0.8);
	p.quadraticTo(w * 0.75, h * 0.8, w * 0.9, h * 0.5);
	p.lineTo(w, h * 0.5);
	p.closeSubPath();
	p.addArrow(Line<float>(w * 0.45, h * 0.95, 0, h * 0.6), 0.0f, w * 0.3, w * 0.42);

	p.startNewSubPath(0, h * 0.5);
	p.quadraticTo(w * 0.2, 0, w * 0.7, h * 0.1);
	p.lineTo(w * 0.6, h * 0.2);
	p.quadraticTo(w * 0.25, h * 0.2, w * 0.1, h * 0.5);
	p.lineTo(0, h * 0.5);
	p.closeSubPath();
	p.addArrow(Line<float>(w * 0.55, h * 0.05, w, h * 0.4), 0.0f, w * 0.3, w * 0.42);

	p.closeSubPath();

	normal.setPath(p);
	over.setPath(p);
	down.setPath(p);
	
	normal.setFill(Colour::fromRGBA(255, 161, 119, 180)); //FFA177
	over.setFill(Colour::fromRGBA(255, 161, 119, 230)); //230, 7, 56
	down.setFill(Colour::fromRGBA(255, 161, 119, 255));

	b.setImages(&normal, &over, &down);
}

void Skin::drawDrawableButton(Graphics &g, DrawableButton &b, bool isMouseOverButton, bool isButtonDown) {
	if (b.getStyle() == DrawableButton::ImageAboveTextLabel && b.getButtonText() == "MENU")
		return;
	else LookAndFeel_V3::drawDrawableButton(g, b, isMouseOverButton, isButtonDown);
}

void Skin::drawLinearSliderBackground(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) {
	if (slider.getSliderStyle() == Slider::LinearHorizontal) {
		float radius = getSliderThumbRadius(slider) * (slider.isMouseOverOrDragging() ? 0.7f : 0.6f), lradius = getSliderThumbRadius(slider);
		g.setColour(slider.isMouseButtonDown() ? Colour(0xFFd6baad) : Colour(0xB8d6baad)); //FFA177
		Path left, right;
		left.addRoundedRectangle(x, y + (height >> 1) - lradius * 0.15, sliderPos - x - radius, lradius * 0.3, 4.0f, 4.0f, true, false, true, false);
		g.fillPath(left);

		g.setColour(Colour(0x96908D8A)); 
		right.addRoundedRectangle(sliderPos + radius, y + (height >> 1) - radius * 0.1, x + width - sliderPos - radius, radius * 0.2, 4.0f, 4.0f, false, true, false, true);
		g.fillPath(right);
	}
	else LookAndFeel_V3::drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void Skin::drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) {
	if (slider.getSliderStyle() == Slider::LinearHorizontal) {
		float radius = getSliderThumbRadius(slider) * (slider.isMouseOverOrDragging() ? 0.7f : 0.6f);
		g.setColour(slider.isMouseButtonDown() ? Colour(0xFFd6baad) : Colour(0xB8d6baad));
		g.fillEllipse(sliderPos - radius, y + (height >> 1) - radius, radius * 2.0f, radius * 2.0f);
	}
	else LookAndFeel_V3::drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void Skin::drawButtonBackground(Graphics & g, Button & b, const Colour & backgroundColour, bool isMouseOverButton, bool isButtonDown) {
	Path p;
	p.addRoundedRectangle(0, 0, b.getWidth(), b.getHeight(), 4.0f, 4.0f, 
		!(b.isConnectedOnLeft() || b.isConnectedOnTop()), 
		!(b.isConnectedOnRight() || b.isConnectedOnTop()),
		!(b.isConnectedOnLeft() || b.isConnectedOnBottom()),
		!(b.isConnectedOnRight() || b.isConnectedOnBottom()));
	g.setColour(b.getToggleState() ? Colour(0xF0593f50) : Colour(0xF0d6baad));
	g.fillPath(p);

	g.setColour(Colour(0xF0593f50));
	g.strokePath(p, PathStrokeType(2.0f));
}

void Skin::drawButtonText(Graphics & g, TextButton & button, bool isMouseOverButton, bool isButtonDown) {
	g.setColour(button.getToggleState() ? Colour(0xFFd6baad) : Colour(0xff593f50));
	Font font(getTextButtonFont(button, button.getHeight()));
	g.setFont(font);
	//g.setFont(button.getToggleState() ? font.boldened() : font);

	const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
	const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

	const int fontHeight = roundToInt(font.getHeight() * 0.6f);
	const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
	const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	const int textWidth = button.getWidth() - leftIndent - rightIndent;

	if (textWidth > 0)
		g.drawFittedText(button.getButtonText(),
			leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
			Justification::centred, 2);
}

void Skin::CircleWindowButton::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) {
	Colour background(isMouseOverButton ? colour : colour.withAlpha(0.6f));

	const float cx = getWidth() * 0.5f, cy = getHeight() * 0.5f;
	const float diam = jmin(cx, cy) * (isButtonDown ? 0.60f : 0.65f);

	if (isButtonDown)
		g.setColour(background.darker(0.07f));
	else
		g.setColour(background);
	g.fillEllipse(cx - diam, cy - diam, diam * 2.0f, diam * 2.0f);

	Colour c(outline);

	if (!isEnabled())
		c = c.withAlpha(0.6f);

	g.setColour(c);
	g.drawEllipse(cx - diam, cy - diam, diam * 2.0f, diam * 2.0f, diam * 0.1f);

	Path& p = getToggleState() ? toggledShape : normalShape;

	g.setColour(main);
	float scale = 0.55f;
	if(isMouseOverButton) g.fillPath(p, p.getTransformToScaleToFit(cx - diam * scale, cy - diam * scale,
		diam * 2.0f * scale, diam * 2.0f * scale, true));
}
