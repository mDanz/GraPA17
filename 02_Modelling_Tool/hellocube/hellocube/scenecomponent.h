#pragma once
#include <list>

class RigidBodyTransformation;

class SceneComponent
{
public:
	SceneComponent(SceneComponent *parent);
	SceneComponent(SceneComponent *parent, std::string *primitive, RigidBodyTransformation *rigidBodyTransform);
	~SceneComponent();

	void addComponent(SceneComponent *child);
	void addComponent(std::string *primitive, RigidBodyTransformation *rigidBodyTransform);
	void removeComponent(SceneComponent *child);
private:
	std::string *m_primitive;
	RigidBodyTransformation *m_rigidBodyTransform;
	SceneComponent *m_parent;
	std::list<SceneComponent> *m_children;
};

