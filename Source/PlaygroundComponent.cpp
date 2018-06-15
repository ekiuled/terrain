#include "PlaygroundComponent.h"

PlaygroundComponent::PlaygroundComponent() {
}

PlaygroundComponent::~PlaygroundComponent() {
	//_openGLContext.detach();
}

void PlaygroundComponent::resized() {
}

void PlaygroundComponent::paint(Graphics & g) {
	g.fillAll(Colours::lightpink);
	getParentComponent()->repaint();
}

//void PlaygroundComponent::newOpenGLContextCreated() {
//}
//
//void PlaygroundComponent::renderOpenGL() {
//}
//
//void PlaygroundComponent::openGLContextClosing() {
//}

void PlaygroundComponent::mouseMove(const MouseEvent & e) {
	getParentComponent()->mouseMove(e);
}

//void PlaygroundComponent::initialise() {
//	openGLContext.
//}
//
//void PlaygroundComponent::shutdown() {
//}
//
//void PlaygroundComponent::render() {
//	repaint();
//}
//
