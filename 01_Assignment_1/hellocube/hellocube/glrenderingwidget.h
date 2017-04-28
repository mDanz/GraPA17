
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets>
#include "logo.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <qopenglvertexarrayobject.h>
#include <qopenglbuffer.h>


class GLRenderingWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit GLRenderingWidget(QWidget* parent  = 0);
	~GLRenderingWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void cleanup();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	void setupVertexAttribs();

	bool m_core;
	int m_xRot;
	int m_yRot;
	int m_zRot;
	QPoint m_lastPos;
	Logo m_logo;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_logoVbo;
	QOpenGLShaderProgram *m_program;
	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;
	int m_lightPosLoc;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;
	bool m_transparent;
};

#endif
