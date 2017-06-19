
#include "viewportmodel.h"
#include "cameramodel.h"

ViewPortModel::ViewPortModel(SceneModel* scene)
{
	m_scene = scene;
	m_cameras[0] = new CameraModel(false, QVector3D(0, -10, -1), QQuaternion::fromAxisAndAngle(0, 0, 1, 0));
	m_cameras[1] = new CameraModel(true, QVector3D(0, 0, -200));
	m_cameras[2] = new CameraModel(true, QVector3D(0, 0, -200), QQuaternion::fromAxisAndAngle(0, 1, 0, 90));
	m_cameras[3] = new CameraModel(true, QVector3D(0, 0, -500), QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
	m_currentCamera = m_cameras[0];
}

ViewPortModel::~ViewPortModel()
{
	for (int i = 0; i < 4; i++)
	{
		delete m_cameras[i];
	}
}

SceneModel* ViewPortModel::getScene() const
{
	return m_scene;
}

CameraModel* ViewPortModel::getCamera(int number) const
{
	return m_cameras[number];
}

void ViewPortModel::setCurrentCamera(CameraModel* camera)
{
	m_currentCamera = camera;
}

CameraModel* ViewPortModel::getCurrentCamera() const
{
	return m_currentCamera;
}
