#pragma once
#include <QObject>
#include "openglgeometrytype.h"


class RigidBodyTransformation;
class SceneItem;

class SceneModel : public QObject
{
	Q_OBJECT

public:
	SceneModel();
	~SceneModel();

	SceneItem* getRoot() const;

	void addItem(OpenGLGeometryType primitiveType, RigidBodyTransformation &rigidBodyTransform, SceneItem *parent = nullptr);
	void selectItem(SceneItem *item);
	SceneItem* getItem(int id) const;
	QList<SceneItem*> getAllItems() const;
	SceneItem* getSelectedItem() const;

	void update();

public slots:
	void deleteSelectedItem();
	///void changeMode(int i); // todo volume rendering mode

private:
	SceneItem *m_root;
	SceneItem *m_selectedItem;

signals:
	void sceneChanged();
	void objectSelected(QString name);
};
