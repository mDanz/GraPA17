#pragma once


#include <QQuaternion>

class CameraModel
{
public:
	explicit CameraModel(bool isOrthographic = false, QVector3D position = QVector3D(), QQuaternion rotation = QQuaternion());
	~CameraModel();
	QMatrix4x4 getCameraMatrix() const;
	bool isOrthographic() const;
	void zoom(int m_wheel_delta);
	void move(const QVector3D *translation);
	void rotate(const QQuaternion& rotation);
	void reset();
private:
	QVector3D m_center;
	bool m_isOrthographic;
	QVector3D m_position;
	QQuaternion m_rotation;
	QVector3D m_defaultPosition;
	QQuaternion m_defaultRotation;
};
