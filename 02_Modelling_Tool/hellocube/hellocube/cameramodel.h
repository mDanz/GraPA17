#pragma once


#include <QVector3D>
#include <QQuaternion>

class CameraModel
{
public:
	explicit CameraModel(bool isOrthographic = false, QVector3D position = QVector3D(), QQuaternion rotation = QQuaternion());
	~CameraModel();
	QMatrix4x4 GetCameraMatrix() const;
	bool isOrthographic() const;
	void zoom(int m_wheel_delta);
	void move(const QVector3D *translation);
	void rotate(const QQuaternion& rotation);
private:
	QVector3D eye;		//todo use look at transformation for matrix generation etc
	QVector3D up;
	QVector3D center;
	bool m_isOrthographic;
	QVector3D m_position;
	QQuaternion m_rotation;
};
