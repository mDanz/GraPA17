#pragma once


#include "OpenGLGeometry.h"

class QOpenGLShaderProgram;

class OpenGLTorus : public OpenGLGeometry
{
public:
	OpenGLTorus();
	virtual ~OpenGLTorus();

	virtual void draw(QOpenGLShaderProgram *program) override;
};