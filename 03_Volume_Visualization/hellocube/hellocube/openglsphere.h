#pragma once

#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLSphere : public OpenGLGeometry
{
public:
	OpenGLSphere();
	virtual ~OpenGLSphere();

	virtual void draw(QOpenGLShaderProgram *program) override;
};
