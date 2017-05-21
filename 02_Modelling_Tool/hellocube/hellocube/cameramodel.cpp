
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

QMatrix4x4 CameraModel::GetCameraMatrix() const
{
	auto matrix = QMatrix4x4();	//todo fix
	matrix.setToIdentity();
	matrix.translate(m_position);
	//matrix.translate(-(m_center - m_position));
	matrix.rotate(m_rotation);
	//matrix.translate(m_center - m_position);
	
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
	m_center += *translation;
	m_position += *translation;
}

void CameraModel::rotate(const QQuaternion& rotation)
{
	//todo rotate around current center update position and rotation
	m_rotation = rotation;
}

void CameraModel::reset()
{
	m_position = m_defaultPosition;
	m_rotation = m_defaultRotation;
}