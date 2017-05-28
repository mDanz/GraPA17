#pragma once


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "OpenGLGeometry.h"

class QOpenGLFunctions_4_4_Compatibility;
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
	//void renderCube(int tesselation);
	//void renderPlane(QVector3D normal, int tesselation, QOpenGLFunctions_4_4_Compatibility *glFunc);

	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;
};
