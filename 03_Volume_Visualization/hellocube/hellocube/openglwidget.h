#pragma once

#include <QtGui/QWindow>
#include <QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_4_Compatibility>
#include <QOpenGLFramebufferObject>

#include <qfileinfo.h>
#include "openglprimitivefactory.h"
#include "cameramodel.h"
#include "sceneitem.h"

class SceneModel;
class OpenGLPrimitiveFactory;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Compatibility
{
	Q_OBJECT

public:
	explicit OpenGLWidget(QWidget *parent = 0);
	//explicit OpenGLWidget(SceneModel *scene, CameraModel *cameraModel, QWidget *parent = 0);
	~OpenGLWidget();

	void setModel(SceneModel *scene, CameraModel* camera);
	CameraModel* getCamera() const;

	void select();
	void deselect();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//void setTesselation(int t);
	//void resetCamera();
	void cleanup();
	//void selectedCameraMode();
	//void selectedObjManipulationMode();
	//void keyPressEvent(QKeyEvent *e) override;
	//void keyReleaseEvent(QKeyEvent* e) override;

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
	//void focusInEvent(QFocusEvent *event) override;
	//void focusOutEvent(QFocusEvent *event) override;

private:
	void initializeFrameBufferObject(int width, int height);
	void initializeSceneShaderProgram();
	void initializeHighlightShaderProgram();
	//void initializeColorPickerShaderProgram();
	//void initializeSceneShaders();
	//void initializeColorPickerShaders();
	void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void paintWithSceneShaderProgram(QList<SceneItem*> *items);
	void paintWithHighlightShaderProgram(QList<SceneItem*>* items);
	//void paintWithColorPickerProgram(QList<SceneItem*> *items);
	//void paintFocusHighlight();
	//void manipulateScene();
	void updateOrthoProjection(int width, int height);
	static GLfloat calculateAspectRatio(int width, int height);
	//void processSelection() const;
	int getIdFromScreen(QPoint pos);

	const GLfloat m_lightPos[4] = { 0.5f, 0.0f, 0.2f, 1.0f };
	const float m_damping = 0.01f;
	const GLfloat m_ka[3] = { .7f, 0.0f, 0.0f };
	const GLfloat m_kd[3] = { .5f, .5f, .5f };
	const GLfloat m_ks[3] = { .8f, .8f, .8f };
	const GLfloat m_specExp = 50.0;
	const QString m_vshFile = ":/Resources/shaders/phong.vsh";
	const QString m_fshFile = ":/Resources/shaders/phong.fsh";
	const QString m_highlight_vshFile = ":/Resources/shaders/highlight.vsh";
	const QString m_highlight_fshFile = ":/Resources/shaders/highlight.fsh";

	//bool m_manipulationModeFlag;
	bool m_isSelected;

	QOpenGLShaderProgram *m_program;
	QOpenGLShaderProgram *m_highlightProgram;
	//QOpenGLShaderProgram *m_colorPickerProgram; 
	QOpenGLFramebufferObject *m_fbo;

	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;
	int m_lightPosLoc;
	int m_ambientColor;
	int m_diffuseColor;
	int m_specularColor;
	int m_specularExp;
	int m_idColor;

	int m_colorPicker_ProjMatrixLoc;
	int m_colorPicker_mvMatrixLoc;
	int m_colorPicker_objId;

	CameraModel* m_cameraModel;
	SceneModel* m_scene;

	QMatrix4x4 m_proj;
	QMatrix4x4 m_world;
	//int m_tessellation;
	//int m_wheelDelta;
	/*QPoint *m_lastPos;
	QVector3D *m_dragTranslation;
	QQuaternion m_dragRotation;
	TrackBall *m_trackBall;*/
	//bool m_isTessellationEnabled;
	//QOpenGLShader* m_vertexShader;
	//QOpenGLShader* m_fragmentShader;
	//QOpenGLShader* m_colorPicker_vertexShader;
	//QOpenGLShader* m_colorPicker_fragmentShader;
	//QImage* m_pickerImage;

	OpenGLPrimitiveFactory *m_primitiveFactory;
};
