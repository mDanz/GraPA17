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

class SceneItem : public QObject
{
	Q_OBJECT

public:

	explicit SceneItem(SceneItem *parent = 0);
	explicit SceneItem(OpenGLGeometryType primitiveType, RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~SceneItem();

	void setParent(SceneItem *parent);
	SceneItem* getParent() const;

	int getChildRow(); 
	int getChildCount() const;
	QList<SceneItem*> getChildren() const;
	SceneItem* getChild(int row) const;
	void appendChild(SceneItem *child);
	void removeChild(SceneItem *child);

	const ObjectID* getId() const;
	SceneItem* getItem(int id);
	QList<SceneItem*> getAllItems() const;

	QString getName() const;
	void setName(QString name);

	RigidBodyTransformation* getRigidBodyTransformation() const;
	void setRigidBodyTransform(RigidBodyTransformation* rigid_body_transformation);

	OpenGLGeometryType getPrimitiveType() const;

	//QVariant data(int column) const;
	//QVariant data(const QModelIndex& index, int role) const;
	//int row() const;
	//void clearChildren() const;


private:
	QString createName() const;

	static int s_cubeID;
	static int s_sphereID;
	static int s_cylinderID; 
	static int s_coneID;
	static int s_torusID;
	static int s_objectID;
	static int s_volumeID;

	SceneItem *m_parent;
	QList<SceneItem*> m_children;

	QString m_name;
	ObjectID* m_id;
	OpenGLGeometryType m_primitiveType;
	RigidBodyTransformation *m_rigidBodyTransform;
};
