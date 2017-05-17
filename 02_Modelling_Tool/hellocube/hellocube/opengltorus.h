#pragma once


#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLTorus : OpenGLGeometry
{
public:
	OpenGLTorus();
	~OpenGLTorus();

	virtual void draw(QOpenGLShaderProgram *program) override;
};