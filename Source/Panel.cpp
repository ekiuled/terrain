#include "Panel.h"

Panel::Panel() {
}

Panel::Panel(int w, int h) {
	setLayoutMode(MultiDocumentPanel::FloatingWindows);
	//setLayoutMode(MultiDocumentPanel::MaximisedWindowsWithTabs);
	addDocument(_options = new OptionsComponent, Colours::indianred, true);
	addDocument(_playground = new PlaygroundComponent, Colours::lightsteelblue, true);

	_options->setBounds(0, 0, w, h);
	_playground->setBounds(0, 0, w, h);
}

Panel::~Panel() {
	closeAllDocuments(true);
}

bool Panel::tryToCloseDocument(Component *c) {
	return true;
}

void Panel::paint(Graphics &g) {
	g.fillAll(Colours::lightyellow);
}
