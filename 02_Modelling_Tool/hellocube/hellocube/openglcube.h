#pragma once


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class OpenGLCube : protected QOpenGLFunctions
{
public:
	OpenGLCube(QObject *parent);
	virtual ~OpenGLCube();

	void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
	void initCubeGeometry(QObject* parent);

	QOpenGLVertexArrayObject *m_vao;
	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;
};
