#include "scenemodel.h"
#include "sceneitem.h"

SceneModel::SceneModel()
{
	m_root = new SceneItem(ObjectID(0), QString("Scene"));
	m_root->setSelected(true);
	setupModelData(m_root);
}

SceneModel::~SceneModel()
{
	delete m_root;
}

QVariant SceneModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) //todo increase this to use more roles.
	{
		return QVariant();
	}

	auto item = static_cast<SceneItem*>(index.internalPointer());
	return item->data(index, role);
}

Qt::ItemFlags SceneModel::flags(const QModelIndex& index) const
{
	return (index.isValid()) ? QAbstractItemModel::flags(index) : 0;
}

QVariant SceneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? m_root->data(section) : QVariant();
}

QModelIndex SceneModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	auto root = !parent.isValid() ? m_root : static_cast<SceneItem*>(parent.internalPointer());
	
	auto child = root->child(row);
	if (child)
	{
		return createIndex(row, column, child);
	}
	return QModelIndex();
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	auto child = static_cast<SceneItem*>(index.internalPointer());
	auto parent = child->parent();

	if (parent == m_root)
	{
		return QModelIndex();
	}
	return createIndex(parent->row(), 0, parent);
}

int SceneModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	auto parentItem = !parent.isValid() ? m_root : static_cast<SceneItem*>(parent.internalPointer());
	return parentItem->childCount();
}

int SceneModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid()) 
	{
		return static_cast<SceneItem*>(parent.internalPointer())->columnCount();
	}
	return m_root->columnCount();
}

void SceneModel::deleteSelectedItem() const
{
	auto item = getSelectedItem();
	auto parent = item->parent();
	if (parent) 
	{
		parent->removeChild(item);
	}
}

SceneItem* SceneModel::getSelectedItem() const
{
	return m_root->getSelectedItem();
}

void SceneModel::addItem(SceneItem * geometry) const
{
	getSelectedItem()->appendChild(geometry);

}

void SceneModel::updateSelectedItem(const QModelIndex& current, const QModelIndex& previous) const
{
	auto previousItem = static_cast<SceneItem*>(previous.internalPointer());
	if (previousItem) 
	{
		previousItem->setSelected(false);
	}
	auto currentItem = static_cast<SceneItem*>(current.internalPointer());
	if (currentItem)
	{
		currentItem->setSelected(true);
	}
}

void SceneModel::setupModelData(SceneItem* parent) const
//todo make this custom
{
	parent->appendChild(new SceneItem(ObjectID(12345), QString("Cube 1"), parent));
	parent->appendChild(new SceneItem(ObjectID(5678), QString("Cube 2"), parent));
	parent->appendChild(new SceneItem(ObjectID(90123), QString("Cube 3"), parent));
}
