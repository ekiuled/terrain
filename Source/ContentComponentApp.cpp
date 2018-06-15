#include "ContentComponent.h"

ContentComponent::ContentComponent()
	: _button(new DrawableButton("the button", DrawableButton::ImageStretched)) {
	setLookAndFeel(_skin);

	openGLContext.setComponentPaintingEnabled(false);

	addChildComponent(_options = new OptionsComponent);
	addAndMakeVisible(_button);

	_skin->drawGhostButton(_button);
	_button->addListener(this);
	_button->setAlwaysOnTop(true);
}

ContentComponent::~ContentComponent() {
	_button->removeListener(this);
	deleteAllChildren();
	shutdownOpenGL();
}

void ContentComponent::paint(Graphics &g) {
	g.fillAll(Colours::aliceblue);
	_button->repaint();
}

void ContentComponent::resized() {
	int w = getWidth();
	int h = getHeight();
	_options->setBounds(getLocalBounds());
	_button->setBounds(0, (h >> 1) - (int)(h * 0.025), (int)(w * 0.01), (int)(h * 0.05));
}

void ContentComponent::mouseMove(const MouseEvent &e) {
	Point<int> position(e.getPosition());
	if (position.getX() < _button->getWidth() * 3)
		_button->setVisible(true);
	else
		_button->setVisible(false);
}

void ContentComponent::buttonClicked(Button *button) {
	static bool optionsVisible = false;
	if (button == _button) {
		optionsVisible ^= 1;
		if (optionsVisible)
			_options->setVisible(true);
		else
			_options->setVisible(false);
	}
}

void ContentComponent::initialise() {
	createShaders();
}

void ContentComponent::shutdown() {
	_shader = nullptr;
	_shape = nullptr;
}

void ContentComponent::render() {
	OpenGLHelpers::clear(Colour::fromFloatRGBA(0.7, 1.0, 0.6, 0.0));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const float desktopScale = (float)openGLContext.getRenderingScale();
	glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));
	_shader->use();
	_shape->draw();

	openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ContentComponent::createShaders() {
	_vertexShader = "#version 330 core\n"
		"attribute vec3 position;\n"
		"varying vec4 vertexColor;\n"
		"\n"
		"void main() {\n"
		"gl_Position = vec4(position, 1.0);\n"
		"//gl_PointSize = 100.0;\n"
		"vertexColor = vec4(0.5, 0.0, 0.2, 1.0);\n"
		"}\n";

	_fragmentShader = "#version 330 core\n"
		"varying vec4 vertexColor;\n"
		"\n"
		"void main() {\n"
		"gl_FragColor = vertexColor;\n"
		"}\n";

	ScopedPointer<OpenGLShaderProgram> tempShader(new OpenGLShaderProgram(openGLContext));
	if (tempShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(_vertexShader))
		&& tempShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(_fragmentShader))
		&& tempShader->link()) {
		_shader = tempShader;
		_shader->use();

		_shape = new Shape(openGLContext);
	}
	else throw 1;
}
