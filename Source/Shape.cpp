#include "Shape.h"

Shape::Shape(OpenGLContext & context, OpenGLShaderProgram& shader)
	: _openGLContext(context)
	, _vertexBuffer(0)
	, _indexBuffer(0)
	, _verticesCount(0)
	, _indicesCount(0)
	, _vertices(nullptr)
	, _indices(nullptr)
	, _backVerticesCount(0)
	, _backIndicesCount(0)
	, _backVertices(nullptr)
	, _backIndices(nullptr)
	, _mode(Modes::FLY)
	, _width(100)
	, _depth(100)
	, _height(500)
	, _drawWidth(_width)
	, _updWidth(_width)
	, _backWidth(_width)
	, _drawDepth(_depth)
	, _updDepth(_depth)
	, _backDepth(_depth)
	, _position(createAttribute(shader, "position"))
	, _projectionMatrix(createUniform(shader, "proj"))
	, _viewMatrix(createUniform(shader, "view"))
	, _trans(createUniform(shader, "trans"))
	, _offset(createUniform(shader, "offset"))
	, _top(createUniform(shader, "top"))
	, _far(createUniform(shader, "far"))
	, _near(createUniform(shader, "near"))
	, _topc(Colour(0xfffff4aa))
	, _farc(Colour(0xff78578f))
	, _nearc(Colour(0xffffa177))
	, _delta(0.05)
	, _block(0.6)
	, _drawLines(false)
	, _startNewGen(true)
	, _backTh(false)
	, _updTh(false)
	, _off(0) {
}

Shape::~Shape() {
	if (_backTh)_backThread.join();
	if (_updTh)_updThread.join();
	deleteBuffers();
	freeData(_vertices, _indices);
	freeData(_backVertices, _backIndices);
	_position = nullptr;
	_projectionMatrix = nullptr;
	_viewMatrix = nullptr;
}

void Shape::init() {
	_openGLContext.extensions.glGenBuffers(1, &_vertexBuffer);
	_openGLContext.extensions.glGenBuffers(1, &_indexBuffer);
}

void Shape::reset() {
	_startNewGen = true;
	if (_updTh)_updThread.join();
	_updThread = std::thread([this] {genDefaultData(); });
	_updTh = true;
}

void Shape::draw() {
	_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	_openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

	int block = _drawDepth * _block;

	if (_updTh && _vertices && _indices) {
		_updThread.join();
		_updTh = false;
		_drawDepth = _updDepth, _drawWidth = _updWidth;

		_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices) * _verticesCount, _vertices, GL_STREAM_DRAW);
		_openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * _indicesCount, _indices, GL_STREAM_DRAW);

		_off = _drawDepth - (block = _drawDepth * _block) - 1;

		if (_backTh) {
			_backThread.join();
			_backTh = false;
		}
		_backThread = std::thread([this] {genData(_backVertices, _backIndices, _backVerticesCount, _backIndicesCount, _backWidth, _backDepth); });
		_backTh = true;
	}

	if (_mode == Modes::FLY && _backTh && _off <= 1 && _vertices && _indices) {
		_backThread.join();
		_backTh = false;
		swapData();
		_drawDepth = _backDepth, _drawWidth = _backWidth;

		_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices) * _verticesCount, _vertices, GL_STREAM_DRAW);
		_openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * _indicesCount, _indices, GL_STREAM_DRAW);

		_off = _drawDepth - (block = _drawDepth * _block) - 1;

		_backThread = std::thread([this] {genData(_backVertices, _backIndices, _backVerticesCount, _backIndicesCount, _backWidth, _backDepth); });
		_backTh = true;
	}

	if (_position) {
		_openGLContext.extensions.glVertexAttribPointer(_position->attributeID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		_openGLContext.extensions.glEnableVertexAttribArray(_position->attributeID);
	}
	if (_projectionMatrix) 
		_projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);
	if (_viewMatrix) 
		_viewMatrix->setMatrix4(getViewMatrix().mat, 1, false);
	if (_trans) 
		_trans->setMatrix4(getTransMatrix().mat, 1, false);
	if (_top)
		_top->set(_topc.getFloatRed(), _topc.getFloatGreen(), _topc.getFloatBlue());
	if (_far)
		_far->set(_farc.getFloatRed(), _farc.getFloatGreen(), _farc.getFloatBlue());
	if (_near)
		_near->set(_nearc.getFloatRed(), _nearc.getFloatGreen(), _nearc.getFloatBlue());


	if (_drawLines) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	{
		int stripeScale = 6 * (_drawWidth - 1), maxDepth = (GLfloat)(_drawDepth - 1) * 0.5;
		static int direction = -1;
		if (_mode == Modes::FLY)
			_off += direction;
		int indOff = _off;
		if (_offset)
			_offset->set(0.0f, 0.0f, ((GLfloat)_off) / (maxDepth * 1.001) * 2);
		glDrawElements(GL_TRIANGLES, block * stripeScale, GL_UNSIGNED_INT, (GLvoid *)(indOff * stripeScale * sizeof(GLuint)));
	}
	if (_drawLines) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (_position)
		_openGLContext.extensions.glDisableVertexAttribArray(_position->attributeID);
}

void Shape::setWidth(const int &w) {
	_width = w;
}

void Shape::setHeight(const int &h) {
	_height = h;
}

void Shape::setDepth(const int &d) {
	_depth = d;
}

void Shape::setLocalBounds(Rectangle<int> b) {
	_localBounds = b;
}

void Shape::setRockiness(const double & newRockiness) {
	_delta = newRockiness;
}

void Shape::setDrawMode(const bool & shouldBeFilled) {
	if (shouldBeFilled)
		_drawLines = false;
	else
		_drawLines = true;
}

void Shape::sendData(Options option, double d, int i, Modes mode, Colour c) {
	if (option == Options::RESET)
		reset();
	else if (option == Options::OCTAVES)
		setOctaves(i);
	else if (option == Options::PERSISTENCE)
		setPersistence(d);
	else if (option == Options::WIDTH)
		setWidth(i);
	else if (option == Options::HEIGHT)
		setHeight(i);
	else if (option == Options::DEPTH)
		setDepth(i);
	else if (option == Options::ROCKINESS)
		setRockiness(d);
	else if (option == Options::MODE)
		setMode(mode);
	else if (option == Options::FILL)
		setDrawMode(true);
	else if (option == Options::LINES)
		setDrawMode(false);
	else if (option == Options::TOPC)
		_topc = c;
	else if (option == Options::NEARC)
		_nearc = c;
	else if (option == Options::FARC)
		_farc = c;
	else throw 1;
}

bool Shape::isFree() {
	return !_updTh;
}

int Shape::getWigth() {
	return _width;
}

int Shape::getHeight() {
	return _height;
}

int Shape::getDepth() {
	return _depth;
}

double Shape::getRockiness() {
	return _delta;
}

void Shape::setMode(const Modes & mode) {
	if (mode == Modes::CHANGE)
		if (_mode == Modes::FLY)
			_mode = Modes::STATIC;
		else
			_mode = Modes::FLY;
	else
		_mode = mode;
}

void Shape::genDefaultData() {
	genData(_vertices, _indices, _verticesCount, _indicesCount, _updWidth, _updDepth);
}

void Shape::genData(GLfloat *&vertices, GLuint *&indices, int &verticesCount, int &indicesCount, int &w, int &d) {
	_mutex.lock();
	static int width = _width, depth = _depth;
	static double height = _height * 0.001, delta = _delta, yoffSt = 0.0;
	if (_startNewGen) { 
		width = _width, depth = _depth, height = _height * 0.001, delta = _delta, yoffSt = 0.0;
		permutate();
		_startNewGen = false;
	}
	else 
		yoffSt -= delta * (depth - (int)(depth * _block) - 1);

	int tempVerticesCount = width * depth * 3;
	GLfloat *tempVertices = new GLfloat[tempVerticesCount];
	GLfloat maxWidth = (GLfloat)(width - 1) * 1.5, maxDepth = (GLfloat)(depth - 1) * 0.5;


	double yoff = yoffSt;
	for (int i = 0; i < depth; ++i) {
		double xoff = 0.0;
		for (int j = 0; j < width * 3; j += 3) {
			tempVertices[i * width * 3 + j] = ((GLfloat)j - maxWidth) / (maxWidth * 1.001) * 2.0f; // x
			tempVertices[i * width * 3 + j + 1] = (GLfloat)(noise(xoff, yoff) * height); // z
			tempVertices[i * width * 3 + j + 2] = ((GLfloat)i - maxDepth) / (maxDepth * 1.001) * 2; // y

			xoff += delta;
		}
		yoff += delta;
	}

	int tempIndicesCount = (width - 1) * (depth - 1) * 6;
	GLuint *tempIndices = new GLuint[tempIndicesCount];
	for (int i = 0; i < depth - 1; ++i)
		for (int j = 0; j < (width - 1) * 6; j += 6) {
			tempIndices[i * (width - 1) * 6 + j] = i * width + (j / 6);
			tempIndices[i * (width - 1) * 6 + j + 1] = i * width + ((j / 6) + 1);
			tempIndices[i * (width - 1) * 6 + j + 2] = (i + 1) * width + ((j / 6) + 1);
			tempIndices[i * (width - 1) * 6 + j + 3] = i * width + (j / 6);
			tempIndices[i * (width - 1) * 6 + j + 4] = (i + 1) * width + ((j / 6) + 1);
			tempIndices[i * (width - 1) * 6 + j + 5] = (i + 1) * width + (j / 6);
		}

	std::swap(tempIndices, indices);
	std::swap(tempVertices, vertices);
	indicesCount = tempIndicesCount, verticesCount = tempVerticesCount;
	w = width, d = depth;
	tempIndicesCount = 0, tempVerticesCount = 0;
	freeData(tempVertices, tempIndices);

	_mutex.unlock();
}

void Shape::swapData() {
	std::swap(_vertices, _backVertices);
	std::swap(_indices, _backIndices);
	std::swap(_verticesCount, _backVerticesCount);
	std::swap(_indicesCount, _backIndicesCount);

	freeData(_backVertices, _backIndices);
}

OpenGLShaderProgram::Attribute * Shape::createAttribute(OpenGLShaderProgram & shader, const char * attribName) {
	if (_openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attribName) < 0)
		return nullptr;
	return new OpenGLShaderProgram::Attribute(shader, attribName);
}

OpenGLShaderProgram::Uniform * Shape::createUniform(OpenGLShaderProgram & shader, const char * uniformName) {
	if (_openGLContext.extensions.glGetUniformLocation(shader.getProgramID(), uniformName) < 0)
		return nullptr;
	return new OpenGLShaderProgram::Uniform(shader, uniformName);
}

Matrix3D<float> Shape::getProjectionMatrix() {
	//return Matrix3D<float>();
	float w = 1.0f;
	float h = w * _localBounds.toFloat().getAspectRatio(false);
	float left = -w, right = w, bottom = -h, top = h, near = 1.0f, far = -1.0f;
	return Matrix3D<float>::fromFrustum(left, right, bottom, top, near, far);
}

Matrix3D<float> Shape::getViewMatrix() {
	Matrix3D<float> view;
	return view.rotation(Vector3D<float>(0.5f, 0.0f, 0.0f));
}

Matrix3D<float> Shape::getTransMatrix() {
	return Matrix3D<float>(Vector3D<float>(0.0f, -0.9f, 0.0f));
}

void Shape::freeData(GLfloat *&vertices, GLuint *&indices) {
	if (vertices)
		delete[] vertices;
	if (indices)
		delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

void Shape::deleteBuffers() {
	_openGLContext.extensions.glDeleteBuffers(1, &_vertexBuffer);
	_openGLContext.extensions.glDeleteBuffers(1, &_indexBuffer);

	_vertexBuffer = 0;
	_indexBuffer = 0;
}
