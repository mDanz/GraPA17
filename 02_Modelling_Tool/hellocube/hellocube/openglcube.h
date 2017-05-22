#pragma once


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "OpenGLGeometry.h"

class OpenGLWidget;

class OpenGLCube : public OpenGLGeometry
{
public:
	OpenGLCube();
	//explicit OpenGLCube(QObject *parent);
	virtual ~OpenGLCube();

	virtual void draw(QOpenGLShaderProgram *program) override;

private:
	void initCubeGeometry();

	QOpenGLVertexArrayObject *m_vao;
	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;
};
