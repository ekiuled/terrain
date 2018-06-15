#include "MainWindow.h"

MainWindow::MainWindow()
	: DocumentWindow(
		JUCEApplication::getInstance()->getApplicationName(),
		Colour(0xff2d2d30),
		DocumentWindow::allButtons)
	, _menu(new DrawableButton("MENU", DrawableButton::ImageAboveTextLabel))
{
	setLookAndFeel(&_skin);

	Component::addAndMakeVisible(_menu);
	_menu->addListener(this);

	setUsingNativeTitleBar(false);
	centreWithSize(getParentWidth() * 0.9, getParentHeight() * 0.9);
	setResizable(true, false);
	setTitleBarTextCentred(false);
	setVisible(true);

	setIcon(ImageCache::getFromMemory(BinaryData::icon_png, BinaryData::icon_pngSize));

	//if (auto peer = getPeer())
		//peer->setIcon(_icon);

	setContentOwned(_contentComponent = new ContentComponent(this), false);
	addKeyListener(_contentComponent);
}

MainWindow::~MainWindow() {
	_menu->removeListener(this);
	removeChildComponent(_menu);
}

void MainWindow::closeButtonPressed() {
	JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::resized() {
	DocumentWindow::resized();
	Rectangle<int> area(getTitleBarArea());
	int x = area.getX() + 4;
	_menu->setBounds(x, area.getY(), area.getHeight() * 4, area.getHeight());
}

void MainWindow::buttonClicked(Button *button) {
	if (button == _menu)
		_contentComponent->getControls()->changeControlsVisible();
}
