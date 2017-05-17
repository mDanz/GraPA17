#pragma once


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "OpenGLGeometry.h"

class OpenGLCube : OpenGLGeometry
{
public:
	OpenGLCube(QObject *parent);
	virtual ~OpenGLCube();

	virtual void draw(QOpenGLShaderProgram *program) override;

private:
	void initCubeGeometry(QObject* parent);

	QOpenGLVertexArrayObject *m_vao;
	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;
};
