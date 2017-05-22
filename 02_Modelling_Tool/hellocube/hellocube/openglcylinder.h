#pragma once

#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLCylinder : public OpenGLGeometry
{
public:
	OpenGLCylinder();
	~OpenGLCylinder();

	virtual void draw(QOpenGLShaderProgram *program) override;
};