#include "sceneitem.h"
#include <QVariant>
#include <QFileSystemModel>

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

void SceneItem::removeChild(SceneItem* child)
{
	m_children.removeOne(child);
}

SceneItem* SceneItem::child(int row) const
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

QVariant SceneItem::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return m_data.value(1);
	}

	return QVariant();
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

bool SceneItem::isSelected()
{
	return m_isSelected;
}

void SceneItem::setSelected(bool status)
{
	m_isSelected = status;
}

SceneItem* SceneItem::getSelectedItem()
{
	if (isSelected())
	{
		return this;
	}
	else
	{
		for (int i = 0; i < m_children.count(); i++)
		{
			if (auto child = m_children.at(i)->getSelectedItem())
			{
				return child;
			}
		}
		return nullptr;
	}
}
