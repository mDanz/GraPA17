#pragma once


#include <QOpenGLBuffer>
#include "OpenGLGeometry.h"

class QOpenGLFunctions_4_4_Compatibility;
class OpenGLWidget;

class OpenGLCube : public OpenGLGeometry
{
public:
	OpenGLCube();
	~OpenGLCube();

	virtual void draw(QOpenGLShaderProgram *program) override;
private:
	void initCubeGeometry();
	void renderCube(int tesselation) const;
	void renderPlane(QVector3D normal, int tesselation, QOpenGLFunctions_4_4_Compatibility *glFunc) const;

	void renderTerrain();
	void initGeometry();

	const float m_step = 1.f;
	const int m_gridSize = 50; //needs to be even number
	const float m_terrainScaling = 128.f;

	GLuint m_vertexBuf;
	GLuint m_indexBuf;
	GLuint m_vao;
/*
	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;*/
};
