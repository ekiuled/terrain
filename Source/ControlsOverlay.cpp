#include "ControlsOverlay.h"

ControlsOverlay::ControlsOverlay(DocumentWindow *win, BackgoundColor *bc)
	: Component("Options")
	, _window(win)
	, _play(new DrawableButton("play", DrawableButton::ImageStretched))
	, _reset(new DrawableButton("reset", DrawableButton::ImageStretched))
	, _shape(nullptr)
	, _fly(new TextButton("FLY"))
	, _static(new TextButton("STATIC"))
	, _lines(new TextButton("LINES"))
	, _fill(new TextButton("FILL"))
	, _help(new HelpButton)
	, _near(new ColourChangeButton("NEAR", Options::NEARC, Colour(0xffffa177)))
	, _top(new ColourChangeButton("TOP", Options::TOPC, Colour(0xfffff4aa)))
	, _far(new ColourChangeButton("FAR", Options::FARC, Colour(0xff78578f)))
	, _backColor(new ColourChangeButton("BACK", Options::BACKGROUND_COLOR, Colour::fromFloatRGBA(0.71f, 0.7f, 0.72f, 0.0f)))
	, _pressedChild(nullptr)
	, _controlsVisible(false) {
	setLookAndFeel(&_skin);

	addChildComponent(_play);
	addChildComponent(_reset);
	addChildComponent(_fly);
	addChildComponent(_static);
	addChildComponent(_lines);
	addChildComponent(_fill);
	addChildComponent(_widthSlider);
	addChildComponent(_depthSlider);
	addChildComponent(_heightSlider);
	addChildComponent(_rockinessSlider);
	addChildComponent(_near);
	addChildComponent(_top);
	addChildComponent(_far);
	addChildComponent(_backColor);
	addAndMakeVisible(_help);

	_backColor->setBackgroundColorCC(bc);

	_fly->setClickingTogglesState(true);
	_static->setClickingTogglesState(true);
	_fly->setRadioGroupId(123);
	_static->setRadioGroupId(123);
	_fly->setToggleState(true, dontSendNotification);

	_fill->setClickingTogglesState(true);
	_lines->setClickingTogglesState(true);
	_fill->setRadioGroupId(456);
	_lines->setRadioGroupId(456);
	_fill->setToggleState(true, dontSendNotification);

	_widthSlider.setRange(2, 1000);
	_widthSlider.setSkewFactorFromMidPoint(500);

	_depthSlider.setRange(4, 1000);
	_depthSlider.setSkewFactorFromMidPoint(500);

	_heightSlider.setRange(0, 1000);
	_heightSlider.setSkewFactorFromMidPoint(500);

	_rockinessSlider.setRange(0.0, 0.9999);
	_rockinessSlider.setSkewFactorFromMidPoint(0.05);

	_widthSlider.setValue(500);
	_depthSlider.setValue(500);
	_heightSlider.setValue(500);
	_rockinessSlider.setValue(0.05);

	_widthLabel.setText("Width", dontSendNotification);
	_widthLabel.attachToComponent(&_widthSlider, true);
	_depthLabel.setText("Depth", dontSendNotification);
	_depthLabel.attachToComponent(&_depthSlider, true);
	_heightLabel.setText("Height", dontSendNotification);
	_heightLabel.attachToComponent(&_heightSlider, true);
	_rockinessLabel.setText("Rockiness", dontSendNotification);
	_rockinessLabel.attachToComponent(&_rockinessSlider, true);
	_colorLabel.setText("Colours ", dontSendNotification);
	_colorLabel.attachToComponent(_near, true);

	_widthSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 110, _widthSlider.getTextBoxHeight());
	_depthSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 110, _depthSlider.getTextBoxHeight());
	_heightSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 110, _heightSlider.getTextBoxHeight());
	_rockinessSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 110, _rockinessSlider.getTextBoxHeight());

	_skin.drawPlayButton(*_play);
	_skin.drawResetButton(*_reset);
	_fly->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
	_static->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);

	_fill->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
	_lines->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);

	_near->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
	_top->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft | Button::ConnectedEdgeFlags::ConnectedOnRight);
	_far->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);

	_play->addListener(this);
	_reset->addListener(this);
	_fly->addListener(this);
	_static->addListener(this);
	_fill->addListener(this);
	_lines->addListener(this);
	_widthSlider.addListener(this);
	_depthSlider.addListener(this);
	_heightSlider.addListener(this);
	_rockinessSlider.addListener(this);
	_help->addListener(this);
	_near->addListener(this);
	_top->addListener(this);
	_far->addListener(this);
	_backColor->addListener(this);
	addKeyListener(this);

	_play->setTooltip("Start/stop the flight");
	_reset->setTooltip("Reset");
}

ControlsOverlay::~ControlsOverlay() {
	_play->removeListener(this);
	_reset->removeListener(this);
	_fly->removeListener(this);
	_static->removeListener(this);
	_fill->removeListener(this);
	_lines->removeListener(this);
	_help->removeListener(this);
	_widthSlider.removeListener(this);
	_depthSlider.removeListener(this);
	_heightSlider.removeListener(this);
	_rockinessSlider.removeListener(this);
	_help->removeListener(this);
	_near->removeListener(this);
	_top->removeListener(this);
	_far->removeListener(this);
	_backColor->removeListener(this);
	removeKeyListener(this);
}

void ControlsOverlay::resized() {
	int w = getWidth();
	int h = getHeight();


	int sliderLeft = 65, y = 5, delta = 25, sliderWidth = 332;
	_widthSlider.setBounds(sliderLeft, y, sliderWidth, 20); //getWidth() >> 2
	_depthSlider.setBounds(sliderLeft, (y += delta), sliderWidth, 20);
	_heightSlider.setBounds(sliderLeft, (y += delta), sliderWidth, 20);
	_rockinessSlider.setBounds(sliderLeft, (y += delta), sliderWidth, 20);

	int stride = 40, x = sliderLeft + 110;

	_play->setBounds((x -= 25), (y += delta), 20, 20);
	_reset->setBounds((x -= 25), y, 20, 20);

	x = sliderLeft + 120;

	_lines->setBounds(x, y, stride, 20);
	_fill->setBounds((x += stride), y, stride, 20);

	stride += 10;
	_static->setBounds((x += stride + 5), y, stride, 20);
	_fly->setBounds((x += stride), y, stride, 20);

	x = sliderLeft + 120;

	_near->setBounds(x, (y += delta), stride, 20);
	_top->setBounds((x += stride), y, stride, 20);
	_far->setBounds((x += stride), y, stride, 20);
	_backColor->setBounds((x += stride + 2), y, stride, 20);

	_help->setBounds(getLocalBounds().reduced(getWidth() >> 2, getHeight() >> 2));
}

void ControlsOverlay::buttonClicked(Button *button) {
	if (button == _play) {
		_shape->sendData(Options::MODE, 0.0, 0, Inter::Modes::CHANGE);
		if (_fly->getToggleState()) {
			_fly->setToggleState(false, dontSendNotification);
			_static->setToggleState(true, dontSendNotification);
		}
		else {
			_fly->setToggleState(true, dontSendNotification);
			_static->setToggleState(false, dontSendNotification);
		}
	}
	else if (button == _reset && _shape->isFree())
		_shape->sendData(Options::RESET);
	else if (button == _fly)
		_shape->sendData(Options::MODE, 0.0, 0, Inter::Modes::FLY);
	else if (button == _static)
		_shape->sendData(Options::MODE, 0.0, 0, Inter::Modes::STATIC);
	else if (button == _lines)
		_shape->sendData(Options::LINES);
	else if (button == _fill)
		_shape->sendData(Options::FILL);
	else if (button == _help)
		_help->setVisible(!_help->isVisible());
}

void ControlsOverlay::mouseDown(const MouseEvent & event) {
	_shape->sendData(Options::MODE, 0.0, 0, Inter::Modes::CHANGE);
	if (_fly->getToggleState()) {
		_fly->setToggleState(false, dontSendNotification);
		_static->setToggleState(true, dontSendNotification);
	}
	else {
		_fly->setToggleState(true, dontSendNotification);
		_static->setToggleState(false, dontSendNotification);
	}
}

void ControlsOverlay::sliderValueChanged(Slider * slider) {
	if (_shape)
		if (slider == &_widthSlider) {
			_widthSlider.setValue((int)_widthSlider.getValue());
			_shape->sendData(Options::WIDTH, 0.0, _widthSlider.getValue());
		}
		else if (slider == &_depthSlider) {
			_depthSlider.setValue((int)_depthSlider.getValue());
			_shape->sendData(Options::DEPTH, 0.0, _depthSlider.getValue());
		}
		else if (slider == &_heightSlider) {
			_heightSlider.setValue((int)_heightSlider.getValue());
			_shape->sendData(Options::HEIGHT, 0.0, _heightSlider.getValue());
		}
		else if (slider == &_rockinessSlider)
			_shape->sendData(Options::ROCKINESS, _rockinessSlider.getValue());
}

void ControlsOverlay::paint(Graphics & g) {
	if (_controlsVisible) {
		Rectangle<int> area(getLocalBounds());
		g.setColour(Colour::fromRGB(200, 200, 200));
		g.drawRect(area.getX(), area.getY(), 400, 160, 2);
		//g.drawRect(area.getX(), area.getY(), (area.getWidth() >> 2) + 70, 160, 2);
		g.setColour(Colour::fromRGBA(0, 0, 0, 170));
		//g.fillRect(area.getX(), area.getY(), (area.getWidth() >> 2) + 70, 160);
		g.fillRect(area.getX(), area.getY(), 400, 160);
	}
}

bool ControlsOverlay::keyPressed(const KeyPress &key, Component *originatingComponent) {
	static int h = _window->getTitleBarHeight();
	if (key.getKeyCode() == 'R' && _shape->isFree())
		_shape->sendData(Options::RESET);
	else if (key == KeyPress::spaceKey) {
		_shape->sendData(Options::MODE, 0.0, 0, Inter::Modes::CHANGE);
		if (_fly->getToggleState()) {
			_fly->setToggleState(false, dontSendNotification);
			_static->setToggleState(true, dontSendNotification);
		}
		else {
			_fly->setToggleState(true, dontSendNotification);
			_static->setToggleState(false, dontSendNotification);
		}
	}
	else if (key == KeyPress::escapeKey)
		getTopLevelComponent()->userTriedToCloseWindow();
	else if (key.getKeyCode() == 'Q')
		changeControlsVisible();
	else if (key.getKeyCode() == 'A') {
		_shape->sendData(Options::LINES);
		if (_fill->getToggleState()) {
			_fill->setToggleState(false, dontSendNotification);
			_lines->setToggleState(true, dontSendNotification);
		}
	}
	else if (key.getKeyCode() == 'D') {
		_shape->sendData(Options::FILL);
		if (_lines->getToggleState()) {
			_lines->setToggleState(false, dontSendNotification);
			_fill->setToggleState(true, dontSendNotification);
		}
	}
	else if (key.getKeyCode() == 'H' || key == KeyPress::F1Key)
		_help->setVisible(!_help->isVisible());
	else if (key == KeyPress::F11Key) {
		if (_window->getTitleBarHeight()) {
			_window->setTitleBarHeight(0);
			_window->setFullScreen(true);
		}
		else {
			_window->setFullScreen(false);
			_window->setTitleBarHeight(h);
		}
	}
	else if (key.getKeyCode() == 'X') {
		_window->setFullScreen(!_window->isFullScreen());
		if (!_window->getTitleBarHeight())
			_window->setTitleBarHeight(h);
	}
	return true;
}

void ControlsOverlay::changeControlsVisible() {
	_controlsVisible ^= 1;
	repaint();
	_play->setVisible(_controlsVisible);
	_reset->setVisible(_controlsVisible);
	_fly->setVisible(_controlsVisible);
	_static->setVisible(_controlsVisible);
	_lines->setVisible(_controlsVisible);
	_fill->setVisible(_controlsVisible);
	_widthSlider.setVisible(_controlsVisible);
	_depthSlider.setVisible(_controlsVisible);
	_heightSlider.setVisible(_controlsVisible);
	_rockinessSlider.setVisible(_controlsVisible);
	_near->setVisible(_controlsVisible);
	_top->setVisible(_controlsVisible);
	_far->setVisible(_controlsVisible);
	_backColor->setVisible(_controlsVisible);
}

void ControlsOverlay::setInter(Inter * i) {
	_shape = i;
	_top->setInter(i);
	_far->setInter(i);
	_near->setInter(i);
}

bool ControlsOverlay::controlsAreVisible() {
	return _controlsVisible;
}
