#pragma once
#include <qlist.h>

class QModelIndex;
class QVariant;

class SceneItem
{
public:
	explicit SceneItem(const QList<QVariant> &data, SceneItem *parent = 0);
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
private:
	QList<SceneItem*> m_children;
	QList<QVariant> m_data;
	/*RigidBodyTransformation *m_rigidBodyTransform;
	QVariant *m_primitive;
	QVariant *m_primitiveName;
	int m_id;*/
	SceneItem *m_parent;
	bool m_isSelected;
};
