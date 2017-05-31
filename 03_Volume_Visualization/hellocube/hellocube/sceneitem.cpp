#include "sceneitem.h"
#include <QFileSystemModel>

int SceneItem::s_cubeID = 0;
int SceneItem::s_sphereID = 0;
int SceneItem::s_cylinderID = 0;
int SceneItem::s_coneID = 0;
int SceneItem::s_torusID = 0;
int SceneItem::s_objectID = 0;
int SceneItem::s_volumeID = 0;


SceneItem::SceneItem(OpenGLPrimitiveType primitiveType, SceneItem* parent)
	: m_parent(parent)
	, m_primitiveType(primitiveType)
	, m_rigidBodyTransform(new RigidBodyTransformation(QVector3D(), QQuaternion()))
{
	m_name = createName();
	m_id = ObjectID::createID();
	//todo make transform with parent matrix
}

SceneItem::SceneItem(OpenGLPrimitiveType primitiveType, RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: SceneItem(primitiveType, parent)
{
	if (rigidBodyTransform)
	{
		m_rigidBodyTransform = rigidBodyTransform;
	}
}

SceneItem::~SceneItem()
{
	delete m_id;
	delete m_rigidBodyTransform;
	qDeleteAll(m_children);

	if (m_parent)
	{
		m_parent->removeChild(this);
	}
}

void SceneItem::setParent(SceneItem* parent)
{
	m_parent = parent;//todo make transformation with parent transform
}

void SceneItem::appendChild(SceneItem *child)
{
	m_children.append(child);
}

void SceneItem::removeChild(SceneItem *child)
{
	m_children.removeOne(child);
}

const ObjectID* SceneItem::getId() const
{
	return m_id;
}

QString SceneItem::getName() const
{
	return m_name;
}

void SceneItem::setName(QString name)
{
	m_name = name;
}

SceneItem* SceneItem::getParent() const
{
	return m_parent;
}

int SceneItem::getChildRow()
{
	return m_parent ? m_parent->getChildren().indexOf(this) : 0;
}

int SceneItem::getChildCount() const
{
	return m_children.size();
}

QList<SceneItem*> SceneItem::getChildren() const
{
	return m_children;
}

SceneItem* SceneItem::getChild(int row) const
{
	return m_children.at(row);
}

OpenGLPrimitiveType SceneItem::getPrimitiveType() const
{
	return m_primitiveType;
}

QString SceneItem::createName() const
{
	switch (m_primitiveType)
	{
	case Cube: 
		return QString("Cube ").append(QString::number(SceneItem::s_cubeID++));
	case Sphere: 
		return QString("Sphere ").append(QString::number(SceneItem::s_sphereID++));
	case Cylinder: 
		return QString("Cylinder ").append(QString::number(SceneItem::s_cylinderID++));
	case Cone: 
		return QString("Cone ").append(QString::number(SceneItem::s_coneID++));
	case Torus: 
		return QString("Torus ").append(QString::number(SceneItem::s_torusID++));
	case Volume: 
		return QString("Volume ").append(QString::number(SceneItem::s_volumeID++));
	case Object:
		return QString("Object ").append(QString::number(SceneItem::s_objectID++));
	case None:
	default: 
		return "";
	}
}

RigidBodyTransformation* SceneItem::getRigidBodyTransformation() const
{
	return m_rigidBodyTransform;
}

void SceneItem::setRigidBodyTransform(RigidBodyTransformation* rigidBodyTransformation)
{
	m_rigidBodyTransform = rigidBodyTransformation;
}

SceneItem* SceneItem::getItem(int id)
{
	if (m_id->getId() == id)
	{
		return this;
	}
	for (auto i = 0; i < m_children.count(); i++)
	{
		if (auto item = m_children.at(i)->getItem(id))
		{
			return item;
		}
	}
	return nullptr;

}

QList<SceneItem*> SceneItem::getAllItems() const
{
	QList<SceneItem*> items;
	for (auto i = 0; i < m_children.count(); i++)
	{
		items.append(m_children.at(i));
		items.append(m_children.at(i)->getAllItems());
	}
	return items;
}
