#pragma once


#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLCone : OpenGLGeometry
{
public:
	OpenGLCone();
	~OpenGLCone();

	virtual void draw(QOpenGLShaderProgram *program) override;
};