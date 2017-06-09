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

	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_normalBuf;
	QOpenGLBuffer m_indexBuf;
};
