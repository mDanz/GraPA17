#pragma once
#include <qlist.h>
#include <QButtonGroup>
#include "objectid.h"
#include "OpenGLGeometry.h"
#include "rigidbodytransformation.h"
#include "openglgeometrytype.h"

class OpenGLCube;
class OpenGLGeometry;
class QModelIndex;
class QVariant;

class SceneItem
{
public:
	explicit SceneItem(const ObjectID &id, QString &name, SceneItem *parent = 0);
	explicit SceneItem(const ObjectID &id, QString &name, OpenGLGeometryType primitiveType, RigidBodyTransformation &rigidBodyTransform, SceneItem *parent = 0);
	~SceneItem();

	void appendChild(SceneItem *child);
	void removeChild(SceneItem *child);
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
	void clearChildren() const;
	const ObjectID getId() const;
	OpenGLGeometryType getPrimitive() const;
	RigidBodyTransformation* getRigidBodyTransformation();
	QList<SceneItem*> getAllItems() const;
private:
	QList<SceneItem*> m_children;
	QString m_name;
	const ObjectID m_id;
	SceneItem *m_parent;
	OpenGLGeometryType m_primitiveType;
	RigidBodyTransformation m_rigidBodyTransform;
	bool m_isSelected;
};
