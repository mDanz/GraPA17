#pragma once
#include <qlist.h>
#include "objectid.h"
#include "OpenGLGeometry.h"
#include "rigidbodytransformation.h"
#include "openglprimitivetype.h"

class OpenGLCube;
class OpenGLGeometry;
class QModelIndex;
class QVariant;

class SceneItem : public QObject
{
	Q_OBJECT

public:

	explicit SceneItem(OpenGLPrimitiveType primitiveType = OpenGLPrimitiveType::None, SceneItem *parent = nullptr);
	explicit SceneItem(OpenGLPrimitiveType primitiveType, RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
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

	OpenGLPrimitiveType getPrimitiveType() const;
	
private:
	QString createName() const;

	static int s_cubeID;
	static int s_sphereID;
	static int s_cylinderID; 
	static int s_coneID;
	static int s_torusID;
	static int s_objectID;
	static int s_volumeID;
	static int s_terrainID;

	SceneItem *m_parent;
	QList<SceneItem*> m_children;

	QString m_name;
	ObjectID* m_id;
	OpenGLPrimitiveType m_primitiveType;
	RigidBodyTransformation *m_rigidBodyTransform;
};
