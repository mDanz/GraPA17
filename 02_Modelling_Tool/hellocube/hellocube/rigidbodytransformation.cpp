#include "rigidbodytransformation.h"
#include <QMatrix4x4>


RigidBodyTransformation::RigidBodyTransformation(QVector3D m_translation, QQuaternion m_rotation)
	: m_translation(m_translation),
	m_rotation(m_rotation)
{
}

RigidBodyTransformation::~RigidBodyTransformation()
{
}

void RigidBodyTransformation::move(QVector3D& translation)
{
	m_translation.setX(translation.x());
	m_translation.setY(-translation.y());
}

void RigidBodyTransformation::rotate(const QQuaternion& rotation)
{
	m_rotation = rotation;
}

QMatrix4x4 RigidBodyTransformation::getWorldMatrix() const
{
	auto matrix = QMatrix4x4();
	matrix.setToIdentity();
	matrix.translate(m_translation);
	matrix.rotate(m_rotation);

	return matrix;
}
