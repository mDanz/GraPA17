#pragma once

#include <QtGui/QWindow>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLFunctions_4_4_Compatibility>

#include "trackball.h"


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Compatibility
{
	Q_OBJECT

public:
	explicit OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	void wireframeShading();
	void flatShading();
	void gouraudShading();
	void phongShading();
	void setTesselation(int t);
	void resetCamera();
	void cleanup();


protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	QPointF pixelPosToViewPos(const QPointF& pos) const;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:

	const GLfloat m_lightPos[4] = { 0.5f, 0.0f, 0.2f, 1.0f };
	const GLdouble m_zNear = 0.01f;
	const GLdouble m_zFar = 1000.0f;
	const GLdouble m_fov = 45.0f;
	const float m_damping = 0.01f;
	const GLfloat m_specColor[4] = { .5, .5, .5, 1 };
	const GLfloat m_specExp = 50.0;

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram *m_program;
	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;
	int m_lightPosLoc;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;
	int m_tessellation;
	int m_wheelDelta;
	QPoint *m_lastPos;
	GLfloat *m_dragTranslation;
	QQuaternion m_dragRotation;
	TrackBall *m_trackBall;

	void initializeShaderProgram();
	void setupVertexAttribs();
	void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void paintWithTessellation();
	void paint(int x = 0, int y = 0, int z = 0);
	void paintWithShaderProgram();
};
