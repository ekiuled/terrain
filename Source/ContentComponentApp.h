#pragma once
#include "OptionsComponent.h"
#include "AdditionalStructs.h"
#include "Skin.h"

class ContentComponent
	: public OpenGLAppComponent
	, public Button::Listener 
{
public:
	ContentComponent();
	~ContentComponent();

	void paint(Graphics &g) override;
	void resized() override;
	void mouseMove(const MouseEvent &e) override;
	void buttonClicked(Button *button) override;

	void initialise() override;
	void shutdown() override;
	void render() override;

private:
	void createShaders();

	ScopedPointer<OpenGLShaderProgram> _shader;
	ScopedPointer<Shape> _shape;

	SafePointer<DrawableButton> _button;
	SafePointer<OptionsComponent> _options;
	SafePointer<Skin> _skin;

	const char *_vertexShader;
	const char *_fragmentShader;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentComponent)
};