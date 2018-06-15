#include "ContentComponent.h"

ContentComponent::ContentComponent(DocumentWindow *win)
	: Component("Content")
	, _background(Colour::fromFloatRGBA(0.71f, 0.7f, 0.72f, 0.0f))
{
	_openGLContext.setRenderer(this);
	_openGLContext.attachTo(*this);
	_openGLContext.setContinuousRepainting(true);
	
	addAndMakeVisible(_controlsOverlay = new ControlsOverlay(win, this));
	_controlsOverlay->setAlwaysOnTop(true);
}

ContentComponent::~ContentComponent() {
	deleteAllChildren();
	_openGLContext.detach();
}

void ContentComponent::paint(Graphics &g) {
}

void ContentComponent::resized() {
	_controlsOverlay->setBounds(getLocalBounds());
}

void ContentComponent::newOpenGLContextCreated() {
	createShaders();
}

void ContentComponent::renderOpenGL() {
	OpenGLHelpers::clear(_background);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_shader->use();
	_shape->setLocalBounds(getLocalBounds());
	_shape->draw();

	_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	_openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ContentComponent::openGLContextClosing() {
}

bool ContentComponent::keyPressed(const KeyPress & key, Component * originatingComponent) {
	_controlsOverlay->keyPressed(key, originatingComponent);
	return true;
}

void ContentComponent::reset() {
	_shape->reset();
}

ControlsOverlay * ContentComponent::getControls() {
	return _controlsOverlay;
}

void ContentComponent::setColor(Colour c) {
	_background = c;
}

void ContentComponent::createShaders() {
	_vertexShader = "#version 330 core\n"
		"attribute vec3 position;\n"
		"varying vec4 vertexColor;\n"
		"varying vec3 pos;\n"

		"uniform mat4 proj;\n"
		"uniform mat4 view;\n"
		"uniform mat4 trans;\n"
		"uniform vec3 offset;\n"
		"uniform vec3 top;\n"
		"uniform vec3 far;\n"
		"uniform vec3 near;\n"
		"\n"

		"void main() {\n"
		"gl_Position = (proj + trans) * view * vec4(position - offset, 1.0);\n"
		"pos = vec3((position.x + 1.0) * 0.5, position.y, (position.z - offset.z));\n"
		//"vertexColor = vec4(pos.y * top.x + pos.z * near.x + (1 - pos.z) * far.x, pos.y * top.y + pos.z * near.y + (1 - pos.z) * far.y, pos.y * top.z + pos.z * near.z + (1 - pos.z) * far.z, 1.0);\n"
		"vertexColor = vec4(pos.y * top.x + (pos.z + 1.0) * 0.5 * near.x - pos.z * far.x, pos.y * top.y + (pos.z + 1.0) * 0.5 * near.y - pos.z * far.y, pos.y * top.z + (pos.z + 1.0) * 0.5 * near.z - pos.z * far.z, 1.0);\n"
		//"vertexColor = vec4((position.z - offset.z + 1.0) * 0.5 * (position.y + 0.7) * 0.6, (position.z - offset.z + 1.0) * 0.5, (position.z - offset.z + 1.0) * 0.5 * (position.y + 0.7) * 0.9, 1.0);\n"
		"}\n";

	_fragmentShader = "#version 330 core\n"
		"varying vec4 vertexColor;\n"
		"\n"
		"void main() {\n"
		"gl_FragColor = vertexColor;\n"
		"}\n";

	OpenGLShaderProgram *tempShader = new OpenGLShaderProgram(_openGLContext);
	if (tempShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(_vertexShader))
		&& tempShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(_fragmentShader))
		&& tempShader->link()) {
		_shader = tempShader;
		_shader->use();

		_shape = new Shape(_openGLContext, *_shader);
		_controlsOverlay->setInter(_shape);
		_shape->setWidth(500);
		_shape->setDepth(500);
		_shape->reset();
		_shape->init();
	}
	else throw 1;
}
