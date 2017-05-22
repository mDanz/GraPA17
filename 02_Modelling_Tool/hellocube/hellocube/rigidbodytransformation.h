#pragma once
#include <QQuaternion>


class RigidBodyTransformation : public QVariant
{
public:
	RigidBodyTransformation(QVector3D m_translation, QQuaternion m_rotation);
	~RigidBodyTransformation();
	void move(QVector3D& translation);
	void rotate(const QQuaternion& rotation);
	QMatrix4x4 getWorldMatrix() const;
private:
	QVector3D m_translation;
	QQuaternion m_rotation;
};
