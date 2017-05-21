#pragma once
#include <qlist.h>
#include <QButtonGroup>
#include "objectid.h"

class QModelIndex;
class QVariant;

class SceneItem
{
public:
	explicit SceneItem(const ObjectID &id, QString &name, SceneItem *parent = 0);
	~SceneItem();

	void appendChild(SceneItem *child);
	void removeChild(SceneItem* child);
	SceneItem *child(int row) const;
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	QVariant data(const QModelIndex& index, int role) const;
	int row() const;
	SceneItem *parent();
	bool isSelected();
	void setSelected(bool status);
	SceneItem* getSelectedItem();
	void clearChildren() const;
	const ObjectID getId() const;

private:
	QList<SceneItem*> m_children;
	/*RigidBodyTransformation *m_rigidBodyTransform;
	QVariant *m_primitive;
	QVariant *m_primitiveName;*/
	QString m_name;
	const ObjectID m_id;
	SceneItem *m_parent;
	bool m_isSelected;
};
