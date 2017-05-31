#pragma once

#include <QtGui/QWindow>
#include <QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_4_Compatibility>
#include <QOpenGLFramebufferObject>

#include <qfileinfo.h>
#include "openglprimitivefactory.h"
#include "cameramodel.h"
#include "sceneitem.h"
#include "volumemodel.h"

class SceneModel;
class OpenGLPrimitiveFactory;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Compatibility
{
	Q_OBJECT

public:
	explicit OpenGLWidget(QWidget *parent = nullptr);
	~OpenGLWidget();

	void setModel(SceneModel *scene, CameraModel* camera);
	CameraModel* getCamera() const;

	void select();
	void deselect();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	void cleanup();

signals:
	void clickedInside();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	QPointF pixelPosToScreenPos(const QPointF& pos) const;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	void initializeFrameBufferObjects(int width, int height);
	void initializeSceneShaderProgram();
	void initializeHighlightShaderProgram();
	void initializeEntryExitShaderProgram();
	void initializeVolumeShaderProgram();
	void paintWithSceneShaderProgram(QList<SceneItem*> *items);
	void paintWithHighlightShaderProgram();
	void paintWithVolumeShaderProgram(QList<SceneItem*> *items);
	void renderEntryPoints();
	void renderEntryExitPoints(VolumeModel* volume);
	void renderExitPoints(VolumeModel* volume);
	void renderVolumeData(VolumeModel* volume);
	int getIdFromScreen(QPoint pos);

	const QVector3D m_lightPos = { 0.5f, 0.0f, 0.2f };
	const float m_damping = 0.01f;
	const QVector3D m_ka = { .4f, 0.0f, 0.4f };
	const QVector3D m_kd = { .6f, .6f, .6f };
	const QVector3D m_ks = { .8f, .8f, .8f };
	const QVector4D m_highlightColor = { .0f, .7f, .0f, .9f };
	const float m_specExp = 50.0;
	const QString m_vshFile = "./Resources/shaders/phong.vsh";
	const QString m_fshFile = "./Resources/shaders/phong.fsh";
	const QString m_highlight_vshFile = "./Resources/shaders/highlight.vsh";
	const QString m_highlight_fshFile = "./Resources/shaders/highlight.fsh";
	const QString m_volume_vshFile = "./Resources/shaders/volume.vsh";
	const QString m_volume_fshFile = "./Resources/shaders/volume.fsh";
	const QString m_entryExit_vshFile = "./Resources/shaders/entryExit.vsh";
	const QString m_entryExit_fshFile = "./Resources/shaders/entryExit.fsh";

	bool m_isSelected;
	int m_displaymode;

	QOpenGLShaderProgram *m_program;
	QOpenGLShaderProgram *m_highlightProgram;
	QOpenGLShaderProgram *m_entryExitProgram;
	QOpenGLShaderProgram *m_volumeShaderProgram;
	QOpenGLFramebufferObject *m_fbo;
	QOpenGLFramebufferObject *m_entryExitFbo;

	GLuint m_boxVbo;
	GLuint m_quadVbo;

	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;
	int m_lightPosLoc;
	int m_ambientColor;
	int m_diffuseColor;
	int m_specularColor;
	int m_specularExp;
	int m_idColor;
	int m_stepLoc;

	int m_colorPicker_ProjMatrixLoc;
	int m_colorPicker_mvMatrixLoc;
	int m_colorPicker_objId;

	CameraModel* m_cameraModel;
	SceneModel* m_scene;

	QMatrix4x4 m_proj;
	QMatrix4x4 m_world;

	OpenGLPrimitiveFactory *m_primitiveFactory;
};
