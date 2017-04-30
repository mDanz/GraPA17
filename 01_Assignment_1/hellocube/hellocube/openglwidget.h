#pragma once

#include <QtGui/QWindow>
#include <QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_4_Compatibility>

#include "trackball.h"

//class QPainter;
//class QOpenGlContext;
//class QOpenGlPainterDevice;

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

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	/*bool m_core;
	bool m_transparent;*/
	QOpenGLContext *m_context;

	int m_tesselation;
	int m_wheelDelta;
	QPoint *m_lastPos;
	QVector2D *m_dragTranslation;
	QQuaternion *m_dragRotation;
	TrackBall *m_trackBall;

	void OpenGLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
};
