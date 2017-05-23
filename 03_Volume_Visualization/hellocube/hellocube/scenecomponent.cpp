//
//#include "scenecomponent.h"
//
//SceneComponent::SceneComponent(SceneComponent *parent)
//	: m_primitive(), m_rigidBodyTransform(nullptr), m_parent(parent)
//{
//	m_children = new std::list<SceneComponent>();
//}
//
//SceneComponent::SceneComponent(SceneComponent* parent, std::string* primitive, RigidBodyTransformation* rigidBodyTransform)
//	: SceneComponent(parent) 
//{
//	m_primitive = primitive;
//	m_rigidBodyTransform = rigidBodyTransform;
//}
//
//SceneComponent::~SceneComponent()
//{
//	delete m_parent;
//	m_parent = nullptr;
//	delete m_children;
//	m_children = nullptr;
//}
//
//void SceneComponent::addComponent(SceneComponent *child)
//{
//	m_children->insert(m_children->end(), child);
//}
//
//void SceneComponent::addComponent(std::string* primitive, RigidBodyTransformation* rigidBodyTransform)
//{
//	m_children->insert(m_children->end(), new SceneComponent(this, primitive, rigidBodyTransform));
//}
//
//void SceneComponent::removeComponent(SceneComponent *child)
//{
//	m_children->remove(child);
//}
