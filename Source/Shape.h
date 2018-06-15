#pragma once
#include "PerlinNoise.h"
#include "Inter.h"
#include <iostream>
#include <thread>
#include <mutex>

class Shape
	: public PerlinNoise
	, public Inter{
public:
	Shape(OpenGLContext &context, OpenGLShaderProgram& shader);
	~Shape();
	void init();
	void reset();
	void draw();

	void setWidth(const int &w);
	void setHeight(const int &h);
	void setDepth(const int &d);
	void setLocalBounds(Rectangle<int> b);
	void setRockiness(const double &newRockiness);
	void setDrawMode(const bool &shouldBeFilled);

	void sendData(Options option, double d = 0.0, int i = 0, Modes mode = Modes::DEFAULT, Colour = Colour(0)) override;
	bool isFree() override;

	int getWigth();
	int getHeight();
	int getDepth();
	double getRockiness();

	void setMode(const Modes &mode);
	void genDefaultData();
	void genData(GLfloat *&vertices, GLuint *&indices, int &verticesCount, int &indicesCount, int &w, int &d);
	void swapData();


private:
	OpenGLShaderProgram::Attribute* createAttribute(OpenGLShaderProgram& shader, const char* attribName);
	OpenGLShaderProgram::Uniform* createUniform(OpenGLShaderProgram& shader, const char* uniformName);

	Matrix3D<float> getProjectionMatrix();
	Matrix3D<float> getViewMatrix();
	Matrix3D<float> getTransMatrix();

	void freeData(GLfloat *&vertices, GLuint *&indices);
	void deleteBuffers();

	std::thread _backThread, _updThread;
	std::mutex _mutex;

	OpenGLContext &_openGLContext;
	GLfloat *_vertices, *_backVertices;
	GLuint *_indices, *_backIndices;
	int _verticesCount, _backVerticesCount;
	int _indicesCount, _backIndicesCount;
	Modes _mode;
	GLuint _vertexBuffer;
	GLuint _indexBuffer;
	int _width;
	int _depth;
	int _height;
	int _drawWidth, _updWidth, _backWidth;
	int _drawDepth, _updDepth, _backDepth;
	ScopedPointer<OpenGLShaderProgram::Attribute> _position;
	ScopedPointer<OpenGLShaderProgram::Uniform> _projectionMatrix, _viewMatrix, _trans, _offset, _top, _far, _near;
	Colour _topc, _nearc, _farc;
	Rectangle<int> _localBounds;
	double _delta;
	double _block;

	bool _drawLines;
	bool _startNewGen;
	bool _backTh, _updTh;
	int _off;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Shape)
};