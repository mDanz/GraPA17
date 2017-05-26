
#include "cameramodel.h"
#include <QMatrix4x4>

CameraModel::CameraModel(bool isOrthographic, QVector3D position, QQuaternion rotation)
{
	m_isOrthographic = isOrthographic;
	m_position = position;
	m_defaultPosition = position;
	m_rotation = rotation;
	m_defaultRotation = rotation;
}

CameraModel::~CameraModel()
{
}

QMatrix4x4 CameraModel::getCameraMatrix() const
{
	auto matrix = QMatrix4x4();
	matrix.setToIdentity();
	matrix.translate(m_position);
	matrix.translate(-(m_center));
	matrix.rotate(m_rotation);
	matrix.translate(m_center);
	
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
	m_center.setX(translation->x());
	m_center.setY(-translation->y());
	m_position.setX(translation->x());
	m_position.setY(-translation->y());
}

void CameraModel::rotate(const QQuaternion& rotation)
{
	m_rotation = rotation;
}

QVector3D* CameraModel::getPointOfInterest()
{
	return &m_center;
}

void CameraModel::reset()
{
	m_position = m_defaultPosition;
	m_rotation = m_defaultRotation;
}
