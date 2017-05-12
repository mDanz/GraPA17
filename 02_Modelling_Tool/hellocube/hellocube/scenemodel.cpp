#include "scenemodel.h"
#include "sceneitem.h"

SceneModel::SceneModel(const QString& data, QObject* parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "Title" << "Summary";
	m_root = new SceneItem(rootData);
	setupModelData(data.split(QString("\n")), m_root);
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
	return item->data(index.column());
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

void SceneModel::setupModelData(const QStringList& lines, SceneItem* parent)	//todo make this custom
{
	QList<SceneItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].at(position) != ' ')
				break;
			position++;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QList<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount() - 1);
					indentations << position;
				}
			}
			else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			parents.last()->appendChild(new SceneItem(columnData, parents.last()));
		}

		++number;
	}
}
