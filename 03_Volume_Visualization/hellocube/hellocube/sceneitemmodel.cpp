#include "sceneitem.h"
#include "openglcube.h"
#include <qabstractitemmodel.h>
#include "sceneitemmodel.h"

SceneItemModel::SceneItemModel(QObject *parent) 
	: QAbstractItemModel(parent)
{
	m_root = static_cast<SceneItem*>(parent);
	//m_root = new SceneItem(ObjectID(0), QString("Scene"));
	//m_root->setSelected(true); //todo fix selection of objects
	//setupModelData(m_root);
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
	//Controller::get()->setName(item, value.toString()); //todo fix name setting.
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
	/*if (parent.isValid()) 
	{
		return static_cast<SceneItem*>(parent.internalPointer())->columnCount();
	}
	return m_root->columnCount();*/
}
//
//void SceneItemModel::deleteSelectedItem() const
//{
//	auto item = getSelectedItem();
//	auto parent = item->getParent();
//	if (parent) 
//	{
//		parent->removeChild(item);
//	}
//}
//
//SceneItem* SceneItemModel::getSelectedItem() const
//{
//	return m_root->getSelectedItem();
//}
//
//void SceneItemModel::addItem(SceneItem * geometry) const
//{
//	getSelectedItem()->appendChild(geometry);
//
//}
//
//void SceneItemModel::updateSelectedItem(const QModelIndex& current, const QModelIndex& previous) const
//{
//	auto previousItem = static_cast<SceneItem*>(previous.internalPointer());
//	if (previousItem) 
//	{
//		previousItem->setSelected(false);
//	}
//	auto currentItem = static_cast<SceneItem*>(current.internalPointer());
//	if (currentItem)
//	{
//		currentItem->setSelected(true);
//	}
//}
//
//void SceneItemModel::updateSelectedItem(int id) const
//{
//	auto previousItem = getSelectedItem();
//	if (previousItem)
//	{
//		previousItem->setSelected(false);
//	}
//	auto currentItem = getItem(id);
//	if (currentItem)
//	{
//		currentItem->setSelected(true);
//	}
//}
//
//SceneItem* SceneItemModel::getItem(int id) const
//{
//	return m_root->getId().getId() == id ? m_root : m_root->getItem(id);
//}
//
//QList<SceneItem*> SceneItemModel::getAllItems() const
//{
//	return m_root->getAllItems();
//}
//
//void SceneItemModel::setupModelData(SceneItem* parent) const
////todo make this custom
//{
//	parent->appendChild(new SceneItem(ObjectID(12345), QString("Cube 1"), OpenGLGeometryType::Cube, RigidBodyTransformation(QVector3D(), QQuaternion()), getParent));
//	parent->appendChild(new SceneItem(ObjectID(99999999), QString("Cube 4"), OpenGLGeometryType::Cube, RigidBodyTransformation(QVector3D(1, 1, 1), QQuaternion()), getParent));
//	parent->appendChild(new SceneItem(ObjectID(5678), QString("Cube 2"), getParent));
//	parent->appendChild(new SceneItem(ObjectID(90123), QString("Cube 3"), getParent));
//	updateSelectedItem(12345);
//}
