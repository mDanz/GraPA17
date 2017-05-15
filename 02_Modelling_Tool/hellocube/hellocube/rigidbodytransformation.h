#pragma once
#include <QQuaternion>


class RigidBodyTransformation : public QVariant
{
public:
	RigidBodyTransformation(QVector3D* m_translation, QQuaternion* m_rotation);
	~RigidBodyTransformation();

private:
	QVector3D *m_translation;
	QQuaternion *m_rotation;
};
