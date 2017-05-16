
#include "modellingtoolmodel.h"
#include "cameramodel.h"

ModellingToolModel::ModellingToolModel()
{
	m_scene = new SceneModel;
	m_cameras[0] = new CameraModel(false, QVector3D(0, 0, -5));
	m_cameras[1] = new CameraModel(true, QVector3D(0, 0, -5));
	m_cameras[2] = new CameraModel(true, QVector3D(0, 0, -5), QQuaternion::fromAxisAndAngle(0, 1, 0, 90));
	m_cameras[3] = new CameraModel(true, QVector3D(0, 0, -5), QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
}

ModellingToolModel::~ModellingToolModel()
{
	delete m_scene;
	delete [] m_cameras; //todo fix warning
}

SceneModel* ModellingToolModel::GetScene() const
{
	return m_scene;
}

CameraModel* ModellingToolModel::GetCamera(int number) const
{
	return m_cameras[number];
}

