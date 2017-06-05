#include "sceneitem.h"
#include <qabstractitemmodel.h>
#include "sceneitemmodel.h"
#include "scenecontroller.h"

SceneItemModel::SceneItemModel(QObject *parent) 
	: QAbstractItemModel(parent)
{
	m_root = static_cast<SceneItem*>(parent);
}

SceneItemModel::~SceneItemModel()
{
}

QVariant SceneItemModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)
	{
		return QVariant();
	}

	auto item = static_cast<SceneItem*>(index.internalPointer());
	return item->getName();
}

bool SceneItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid())
	{
		return false;
	}

	auto item = static_cast<SceneItem*>(index.internalPointer());
	SceneController::getController()->setName(item, value.toString());
	return true;
}

Qt::ItemFlags SceneItemModel::flags(const QModelIndex& index) const
{
	return (index.isValid()) ?	Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled : 0;
}

Qt::DropActions SceneItemModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QVariant SceneItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? QString("Scene") : QVariant();
}

QModelIndex SceneItemModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	auto root = !parent.isValid() ? m_root : static_cast<SceneItem*>(parent.internalPointer());
	
	auto child = root->getChild(row);
	if (child)
	{
		return createIndex(row, column, child);
	}
	return QModelIndex();
}

QModelIndex SceneItemModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	auto child = static_cast<SceneItem*>(index.internalPointer());
	auto parent = child->getParent();

	if (parent == m_root)
	{
		return QModelIndex();
	}
	return createIndex(parent->getChildRow(), 0, parent);
}

int SceneItemModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	auto parentItem = !parent.isValid() ? m_root : static_cast<SceneItem*>(parent.internalPointer());
	return parentItem ? parentItem->getChildCount() : 0;
}

int SceneItemModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}


