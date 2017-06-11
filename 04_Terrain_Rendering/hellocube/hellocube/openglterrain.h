#pragma once
#include "openglgeometry.h"
#include <QOpenGLBuffer>


class OpenGLTerrain : public OpenGLGeometry
{
public:
	OpenGLTerrain();
	~OpenGLTerrain();

	virtual void draw(QOpenGLShaderProgram *program) override;

private:

	void initCubeGeometry();

	const float m_step = 1.f;
	const int m_gridSize = 50; //needs to be even number

	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_indexBuf;
};

