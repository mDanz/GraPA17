#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


class OpenGLGeometry : protected QOpenGLFunctions
{
public:
	virtual ~OpenGLGeometry() {}

	virtual void draw(QOpenGLShaderProgram *program) {}
};
