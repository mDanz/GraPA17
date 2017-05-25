#pragma once
#include <qabstractitemmodel.h>
#include "openglgeometry.h"


class SceneItem;

class SceneItemModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit SceneItemModel(QObject *parent = nullptr);
	~SceneItemModel();

	QVariant data(const QModelIndex &index, int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	Qt::DropActions supportedDropActions() const override ;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	
	/*void deleteSelectedItem() const;
	SceneItem* getSelectedItem() const;
	void addItem(SceneItem *geometry) const;
	void updateSelectedItem(const QModelIndex& current, const QModelIndex& previous) const;
	void updateSelectedItem(int id) const;
	SceneItem* getItem(int id) const;
	QList<SceneItem*> getAllItems() const;*/

private:
	void setupModelData(SceneItem *parent) const;

	SceneItem *m_root;
};

