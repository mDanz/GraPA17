#include "sceneitem.h"
#include <QVariant>
#include <QFileSystemModel>


SceneItem::SceneItem(const ObjectID& id, QString& name, SceneItem* parent)
	: m_name(name)
	, m_id(id)
	, m_parent(parent)
	, m_isSelected(false)
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
	return 5;
	
}

QVariant SceneItem::data(int column) const
{
	switch (column)
	{
	case 0:
		return m_name;
	default:
		return QVariant();
	}
}

QVariant SceneItem::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return m_name;
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

void SceneItem::clearChildren() const
{
	qDeleteAll(m_children);
}

const ObjectID SceneItem::getId() const
{
	return m_id;
}
