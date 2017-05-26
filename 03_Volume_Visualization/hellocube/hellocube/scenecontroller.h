#pragma once
#include <QObject>
#include "openglgeometrytype.h"
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

	void click(OpenGLWidget* widget, QPointF screenPos, QPointF worldPos);
	void release(OpenGLWidget* widget, QPointF screenPos, QPointF worldPos);
	void move(OpenGLWidget* widget, QLineF screenDelta, QLineF worldDelta, bool leftBtn, Qt::KeyboardModifiers mods);
	void wheel(OpenGLWidget* widget, float delta, Qt::KeyboardModifiers mods);

public slots:
	void cameraModeSelected();
	void objectModeSelected();
	void setTessellation(int t);

	void selectedItemChanged(const QModelIndex &current, const QModelIndex &previous) const;

	void cubeAdded() const;
	void sphereAdded() const;
	void cylinderAdded() const;
	void coneAdded() const;
	void torusAdded() const;
	void volumeAdded() const;

private:
	SceneController();
	void addItem(OpenGLGeometryType type) const;

	bool m_isInCameraMode;
	int m_tessellation;
	TrackBall *m_trackball;
	bool simpleClick;

	SceneModel *m_scene;
	ViewPortWidget* m_viewPortWidget;

	// used for interactive step distance in volume rendering
	//float oldStep; //todo volume rendering

	static SceneController *s_controller;
};
