#pragma once
#include "ControlsOverlay.h"
#include "Skin.h"
#include "Shape.h"

class ContentComponent
	: public Component
	, private OpenGLRenderer
	, public KeyListener
	, public BackgoundColor
{
public:
	ContentComponent(DocumentWindow *win);
	~ContentComponent();

	void paint(Graphics &g) override;
	void resized() override;

	void newOpenGLContextCreated() override;
	void renderOpenGL() override;
	void openGLContextClosing() override;

	bool keyPressed(const KeyPress &key, Component *originatingComponent) override;

	void reset();
	ControlsOverlay *getControls();

	void setColor(Colour c) override;

private:
	void createShaders();

	OpenGLContext _openGLContext;
	ScopedPointer<OpenGLShaderProgram> _shader;
	ScopedPointer<Shape> _shape;

	SafePointer<ControlsOverlay> _controlsOverlay;

	const char *_vertexShader;
	const char *_fragmentShader;

	Colour _background;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentComponent)
};