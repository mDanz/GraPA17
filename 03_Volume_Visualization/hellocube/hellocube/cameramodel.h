#pragma once

#include <QQuaternion>


class CameraModel
{
public:
	explicit CameraModel(bool isOrthographic = false, QVector3D position = QVector3D(), QQuaternion rotation = QQuaternion());
	~CameraModel();
	QMatrix4x4* getCameraMatrix();
	void setProjection(bool isOrthographic, float fov, float zNear, float zFar);
	void resizeViewPort(int width, int height);
	QMatrix4x4* getProjectionMatrix();
	bool isOrthographic() const;
	void zoom(int m_wheel_delta);
	void move(const QVector3D *translation);
	void rotate(const QQuaternion& rotation);
	QVector3D* getPointOfInterest();
	void reset();
private:
	void updateOrthoProjection();

	const float m_damping = 0.01f;
	
	float m_zNear;
	float m_zFar;
	float m_fov;
	float m_aspect;

	QVector3D m_center;
	bool m_isOrthographic;
	QVector3D m_position;
	QQuaternion m_rotation;
	QVector3D m_defaultPosition;
	QQuaternion m_defaultRotation;
	QMatrix4x4 *m_projectionMatrix;
	QMatrix4x4 *m_cameraMatrix;
};
