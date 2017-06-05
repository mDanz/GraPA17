#pragma once

#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLCylinder : public OpenGLGeometry
{
public:
	OpenGLCylinder();
	virtual ~OpenGLCylinder();

	virtual void draw(QOpenGLShaderProgram *program) override;
};