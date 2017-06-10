
#include "cameramodel.h"
#include <QMatrix4x4>

CameraModel::CameraModel(bool isOrthographic, QVector3D position, QQuaternion rotation)
	: m_projectionMatrix(nullptr)
	, m_cameraMatrix(nullptr)
{
	m_isOrthographic = isOrthographic;
	m_defaultPosition = position;
	m_position = new QVector3D(m_defaultPosition);
	m_defaultRotation = rotation;
	m_rotation = new QQuaternion(m_defaultRotation);
	setProjection(m_isOrthographic, 45.0f, 0.01f, 1000.f);
}

CameraModel::~CameraModel()
{
	delete m_cameraMatrix;
	delete m_projectionMatrix;
	delete m_position;
	delete m_center;
	delete m_rotation;
}

QMatrix4x4* CameraModel::getCameraMatrix()
{
	if (!m_cameraMatrix)
	{
		m_cameraMatrix = new QMatrix4x4();
	}
	m_cameraMatrix->setToIdentity();
	//m_cameraMatrix->lookAt(m_position, m_center, m_rotation * QVector3D(0, 1, 0));
	m_cameraMatrix->translate(*m_position);
	m_cameraMatrix->translate(-(*m_center));
	m_cameraMatrix->rotate(*m_rotation);
	m_cameraMatrix->translate(*m_center);
	
	return m_cameraMatrix;
}

void CameraModel::setProjection(bool isOrthographic, float fov, float zNear, float zFar)
{
	m_isOrthographic = isOrthographic;
	m_fov = fov;
	m_zNear = zNear;
	m_zFar = zFar;
}

void CameraModel::resizeViewPort(int width, int height)
{
	m_aspect = float(width) / (height ? height : 1);
}

QMatrix4x4* CameraModel::getProjectionMatrix()
{
	if (!m_projectionMatrix)
	{
		m_projectionMatrix = new QMatrix4x4();
	}

	m_projectionMatrix->setToIdentity();
	if(!isOrthographic())
	{
		m_projectionMatrix->perspective(m_fov, m_aspect, m_zNear, m_zFar);
	}
	else
	{
		updateOrthoProjection();
	}

	return m_projectionMatrix;
}

bool CameraModel::isOrthographic() const
{
	return m_isOrthographic;
}

void CameraModel::zoom(int m_wheelDelta) const
{
	m_position->setZ(m_wheelDelta * m_damping + m_position->z());
}

void CameraModel::moveTo(const QVector3D* translation) const
{
	m_center->setX(translation->x() * m_damping);
	m_center->setY(-translation->y() * m_damping);
	m_position->setX(translation->x() * m_damping);
	m_position->setY(-translation->y() * m_damping);
}

void CameraModel::move(const QVector3D* translation) const
{
	m_center->setX(m_center->x() + translation->x() * m_damping);
	m_center->setY(m_center->y() + translation->y() * m_damping);
	m_center->setZ(m_center->z() + translation->z() * m_damping);
	m_position->setX(m_position->x() + translation->x() * m_damping);
	m_position->setY(m_position->y() + translation->y() * m_damping);
	m_position->setZ(m_position->z() + translation->z() * m_damping);

}

void CameraModel::rotate(const QQuaternion& rotation)
{
	auto tmp = QQuaternion(-rotation.scalar(), *m_rotation * rotation.vector()) * *m_rotation;
	delete m_rotation;
	m_rotation = &tmp;
	//m_rotation *= rotation;
}

QVector3D* CameraModel::getPointOfInterest() const
{
	return m_center;
}

void CameraModel::reset()
{
	delete m_position;
	m_position = new QVector3D(m_defaultPosition);
	delete m_rotation;
	m_rotation = new QQuaternion(m_defaultRotation);
	delete m_center;
	m_center = new QVector3D();
}

void CameraModel::updateOrthoProjection() const
{
	float top, bottom, left, right;

	if (m_aspect > 1.0)
	{
		top = 1.0f;
		bottom = -top;
		right = top * m_aspect;
		left = -right;
	}
	else
	{
		right = 1.0f;
		left = -right;
		top = right / m_aspect;
		bottom = -top;
	}
	m_projectionMatrix->ortho(left, right, bottom, top, m_zNear, m_zFar);
}
