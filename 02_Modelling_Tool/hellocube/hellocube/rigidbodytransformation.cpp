#include "rigidbodytransformation.h"


RigidBodyTransformation::RigidBodyTransformation(QVector3D* m_translation, QQuaternion* m_rotation)
	: m_translation(m_translation),
	m_rotation(m_rotation)
{
}

RigidBodyTransformation::~RigidBodyTransformation()
{
	delete m_translation;
	m_translation = nullptr;

	delete m_rotation;
	m_rotation = nullptr;
}
