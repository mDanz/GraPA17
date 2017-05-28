#pragma once
#include <QObject>
#include "openglprimitivetype.h"
#include "viewportwidget.h"
#include "trackball.h"


class OpenGLWidget;
class SceneItem;
class SceneModel;

class SceneController : public QObject
{
	Q_OBJECT

public:
	~SceneController();
	static SceneController* getController();

	void setModelView(SceneModel *scene, ViewPortWidget *viewPortWidget);

	void setName(SceneItem *item, QString name);

	void mousePressed(QPointF screenPos, QPointF mousePos, QMouseEvent* event);
	void mouseReleased(QPointF screenPos, int idUnderMouse, QMouseEvent *event);
	void mouseMoved(QPointF screenPos, QPointF mousePos, QMouseEvent* event);
	void wheelMoved(int wheelDelta) const;

public slots:
	void resetCamera() const;
	void cameraModeSelected();
	void objectModeSelected();
	void setTessellation(int t);

	void singleViewActivated() const;
	void dualViewActivated() const;
	void quadViewActivated() const;

	void selectedItemChanged(const QModelIndex &current, const QModelIndex &previous) const;

	void cubeAdded() const;
	void sphereAdded() const;
	void cylinderAdded() const;
	void coneAdded() const;
	void torusAdded() const;
	void volumeAdded() const;

private:
	SceneController();
	void addItem(OpenGLPrimitiveType type) const;
	void manipulateCamera(QPointF screenPos, QVector3D* delta, QMouseEvent* event) const;
	void manipulateSelectedItem(QPointF screenPos, QVector3D* delta, QMouseEvent* event) const;

	bool m_isInCameraMode;
	int m_tessellation;
	TrackBall *m_trackBall;
	QPointF *m_lastPos;
	bool m_singleClick;

	SceneModel *m_scene;
	ViewPortWidget* m_viewPortWidget;

	// used for interactive step distance in volume rendering
	//float oldStep; //todo volume rendering

	static SceneController *s_controller;
};
