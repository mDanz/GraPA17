#pragma once

#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLSphere : OpenGLGeometry
{
public:
	OpenGLSphere();
	~OpenGLSphere();

	virtual void draw(QOpenGLShaderProgram *program) override;
};
