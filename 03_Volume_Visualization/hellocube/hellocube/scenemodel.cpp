
#include "scenemodel.h"
#include "sceneitem.h"

SceneModel::SceneModel()
	: m_selectedItem(nullptr) 
{
	m_root = new SceneItem();
}

SceneModel::~SceneModel()
{
	delete m_root;
}

SceneItem* SceneModel::getRoot() const
{
	return m_root;
}

void SceneModel::addItem(OpenGLGeometryType primitiveType, RigidBodyTransformation& rigidBodyTransform, SceneItem* parent)
{
	if (!parent)
	{
		parent = m_root;
	}

	//QMatrix4x4 pMat = parent->getGlobalMatrix().inverted(); //todo transform rigid body with parent matrix
	//data.translation = QVector3D(pMat * QVector4D(data.translation, 1));

	auto item = new SceneItem(primitiveType, rigidBodyTransform, parent);
	parent->appendChild(item);

	selectItem(item);
	update();
}

void SceneModel::selectItem(SceneItem* item)
{
	m_selectedItem = item;
	emit objectSelected(!m_selectedItem ? QString("<none>") : m_selectedItem->getName());
}

SceneItem* SceneModel::getItem(int id) const
{
	return m_root->getId()->getId() == id ? m_root : m_root->getItem(id);
}

QList<SceneItem*> SceneModel::getAllItems() const
{
	return m_root->getAllItems();
}

SceneItem* SceneModel::getSelectedItem() const
{
	return m_selectedItem;
}

void SceneModel::update()
{
	//if (!volumeAdded && volume->isReady()) { //todo when i have volume
	//	m_root->addChild(volume);
	//	volumeAdded = true;
	//}

	emit sceneChanged();
}

void SceneModel::deleteSelectedItem()
{
	if (!m_selectedItem) //todo for volume || m_selectedItem == volume)
	{
		return;
	}

	delete m_selectedItem;
	m_selectedItem = nullptr;
	update();
}
