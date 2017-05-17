#pragma once

#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLCylinder : OpenGLGeometry
{
public:
	OpenGLCylinder();
	~OpenGLCylinder();

	virtual void draw(QOpenGLShaderProgram *program) override;
};