#pragma once
#include <QObject>
#include "openglprimitivetype.h"
#include "volumemodel.h"


class RigidBodyTransformation;
class SceneItem;

class SceneModel : public QObject
{
	Q_OBJECT

public:
	SceneModel();
	~SceneModel();

	SceneItem* getRoot() const;

	void addItem(OpenGLPrimitiveType primitiveType, RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	void addItem(SceneItem *item, SceneItem *parent = nullptr);
	void selectItem(SceneItem *item);
	SceneItem* getItem(int id) const;
	QList<SceneItem*> getAllItems() const;
	SceneItem* getSelectedItem() const;
	void setVolume(VolumeModel* volume);
	VolumeModel* getVolume();

	void update();

public slots:
	void deleteSelectedItem();
	///void changeMode(int i); // todo volume rendering mode

private:
	SceneItem *m_root;
	SceneItem *m_selectedItem;
	VolumeModel * m_volume;

signals:
	void sceneChanged();
	void itemSelected(QString name);
};
