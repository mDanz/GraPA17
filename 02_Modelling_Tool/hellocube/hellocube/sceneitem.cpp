#include "sceneitem.h"
#include <QVariant>

SceneItem::SceneItem(const QList<QVariant>& data, SceneItem* parent)
	: m_data(data)
	, m_parent(parent)
{

}

SceneItem::~SceneItem()
{
	qDeleteAll(m_children);
}

void SceneItem::appendChild(SceneItem* child)
{
	m_children.append(child);
}

SceneItem* SceneItem::child(int row)
{
	return m_children.value(row);
}

int SceneItem::childCount() const
{
	return m_children.count();
}

int SceneItem::columnCount() const
{
	return m_data.count();
}

QVariant SceneItem::data(int column) const
{
	return m_data.value(column);
}

int SceneItem::row() const
{
	if (m_parent)
	{
		return m_parent->m_children.indexOf(const_cast<SceneItem*>(this));
	}
	return 0;
}

SceneItem* SceneItem::parent()
{
	return m_parent;
}
