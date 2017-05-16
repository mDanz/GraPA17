
#include "cameramodel.h"
#include <QMatrix4x4>

CameraModel::CameraModel(bool isOrthographic, QVector3D position, QQuaternion rotation)
{
	m_isOrthographic = isOrthographic;
	m_position = position;
	m_rotation = rotation;
}

CameraModel::~CameraModel()
{
}

QMatrix4x4 CameraModel::GetCameraMatrix() const
{
	auto matrix = QMatrix4x4();
	matrix.setToIdentity();
	matrix.translate(m_position);
	matrix.rotate(m_rotation);
	return matrix;
}

bool CameraModel::isOrthographic() const
{
	return m_isOrthographic;
}

void CameraModel::zoom(int m_wheelDelta)
{
	m_position.setZ(m_wheelDelta + m_position.z());
}

void CameraModel::move(const QVector3D* translation)
{
	//todo calculate rotation and position new when center point changes
}

void CameraModel::rotate(const QQuaternion& rotation)
{
	//todo rotate around current center update position and rotation
}
