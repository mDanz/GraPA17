#pragma once
#include <qlist.h>

class QVariant;

class SceneItem
{
public:
	explicit SceneItem(const QList<QVariant> &data, SceneItem *parent = 0);
	~SceneItem();

	void appendChild(SceneItem *child);

	SceneItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	SceneItem *parent();

private:
	QList<SceneItem*> m_children;
	QList<QVariant> m_data;
	SceneItem *m_parent;
};
