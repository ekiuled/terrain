#include "Etc.h"

void SliderLabel::paint(Graphics & g) {
	Rectangle<int> area = getLocalBounds();
	g.setColour(Colour::fromRGBA(180, 180, 180, 200));
	g.drawText(getText(), area, Justification::centredRight);
}

HelpButton::HelpButton()
	: _n(8)
	, _m(3)
	, _keys("H/F1\nQ\nR\nSpace\nA/D\nF11\nX\nEsc\n")
	, _text("Show/hide help\n"
		"Show/hide options\n"
		"Reset\n"
		"Fly/pause\n"
		"Draw lines/fill\n"
		"Fullscreen (kinda)\n"
		"Maximize/restore down\n"
		"Exit")
	, _mouseStuff("Click on the icon to show/hide options."
		"Click on this panel to close it."
		"Click anywhere (but this panel, obviously) to fly/pause.") {
}

void HelpButton::paint(Graphics & g) {
	Rectangle<int> area(2, 2, getWidth() - 4, getHeight() - 4);
	g.setColour(Colour::fromRGB(200, 200, 200));
	g.drawRoundedRectangle(2, 2, getWidth() - 4, getHeight() - 4, 5.0f, 2);
	g.setColour(Colour::fromRGBA(200, 200, 200, 150));
	g.drawLine(10, getHeight() * 0.2, getWidth() - 10, getHeight() * 0.2);
	g.drawLine(area.getWidth() * 0.6, area.getHeight() * 0.2 + 10, area.getWidth() * 0.6, area.getHeight() - 10);
	g.setColour(Colour::fromRGBA(0, 0, 0, 170));
	g.fillRoundedRectangle(2, 2, getWidth() - 4, getHeight() - 4, 5.0f);

	g.setColour(Colour::fromRGB(220, 220, 220));
	g.setFont(Font(30, Font::plain));
	Rectangle<int> k(area), m(area);
	m = k.removeFromRight(area.getWidth() * 0.4);
	k.removeFromBottom(area.getHeight() * 0.8);
	m.removeFromBottom(area.getHeight() * 0.8);
	g.drawFittedText("Keyboard Shortcuts", k, Justification::centred, 2);
	g.drawFittedText("Mouse Tricks", m, Justification::centred, 2);

	k = area, m = area;
	k.removeFromTop(area.getHeight() * 0.2), m.removeFromTop(area.getHeight() * 0.2);
	m = k.removeFromRight(area.getWidth() * 0.4);
	m.reduce(m.getWidth() * 0.1, 0);
	k.reduce(k.getWidth() * 0.1, 0);
	g.setFont(Font(jmin(20.0, k.getWidth() * 0.66 / 10), Font::plain));
	g.drawFittedText(_mouseStuff, m, Justification::centredLeft, 100);
	g.drawFittedText(_text, (m = k.removeFromRight(k.getWidth() * 0.66)), Justification::centredLeft, _n, 1.0f);
	g.drawFittedText(_keys, k, Justification::centredLeft, _n, 1.0f);
}

ColourChangeButton::ColourChangeButton(const String & s, Options option, Colour c)
	: TextButton(s)
	, _opt(option)
	, _currColor(c)
	, _cc(nullptr)
	, _shape(nullptr){
	setSize(10, 20);
	changeWidthToFitText();
}

void ColourChangeButton::clicked() {
	setToggleState(true, dontSendNotification);
	ColorSelectorAndBroadcaster *colourSelector = new ColorSelectorAndBroadcaster(ColourSelector::showColourAtTop | ColourSelector::showSliders | ColourSelector::showColourspace, this);
	colourSelector->setName(this->getName());
	colourSelector->setCurrentColour(_currColor);
	colourSelector->ColourSelector::addChangeListener(this);
	colourSelector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
	colourSelector->setSize(250, 300);

	CallOutBox &cob = CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
	cob.setDismissalMouseClicksAreAlwaysConsumed(true);
}

void ColourChangeButton::changeListenerCallback(ChangeBroadcaster * source) {
	if (ColorSelectorAndBroadcaster* cs = dynamic_cast<ColorSelectorAndBroadcaster*> (source)) {
		_currColor = cs->getCurrentColour();
		if (_opt == Options::BACKGROUND_COLOR)
			_cc->setColor(_currColor);
		else
			_shape->sendData(_opt, 0.0, 0, Inter::Modes::DEFAULT, _currColor);
	}
}

void ColourChangeButton::setInter(Inter * i) {
	_shape = i;
}

void ColourChangeButton::setBackgroundColorCC(BackgoundColor * bc) {
	_cc = bc;
}

void ColourChangeButton::kill() {
	setToggleState(false, dontSendNotification);
}

Colour ColourChangeButton::getColor() {
	return _currColor;
}

ColorSelectorAndBroadcaster::ColorSelectorAndBroadcaster(int flags, ColourChangeButton * button)
	: _parent(button)
	, ColourSelector(flags)
{
}

ColorSelectorAndBroadcaster::~ColorSelectorAndBroadcaster() {
	_parent->kill();
}
