#pragma once


#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLCone : public OpenGLGeometry
{
public:
	OpenGLCone();
	virtual ~OpenGLCone();

	virtual void draw(QOpenGLShaderProgram *program) override;
};