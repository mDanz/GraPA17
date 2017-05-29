
#include "scenemodel.h"
#include "sceneitem.h"

SceneModel::SceneModel()
	: m_selectedItem(nullptr)
	, m_displayMode(DirectRendering)
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

void SceneModel::addItem(OpenGLPrimitiveType primitiveType, RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
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

void SceneModel::addItem(SceneItem *item, SceneItem *parent)
{
	if (!parent)
	{
		parent = m_root;
	}
	item->setParent(parent);
	parent->appendChild(item);

	selectItem(item);
	update();
}

void SceneModel::selectItem(SceneItem* item)
{
	m_selectedItem = item;
	emit itemSelected(!m_selectedItem ? QString("<none>") : m_selectedItem->getName());
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

SceneModel::DisplayMode SceneModel::getDisplayMode() const
{
	return m_displayMode;
}

void SceneModel::update()
{
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

void SceneModel::changeDisplayMode(DisplayMode mode)
{
	m_displayMode = mode;
	emit sceneChanged();
}
