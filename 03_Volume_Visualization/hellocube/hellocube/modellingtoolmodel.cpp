
#include "modellingtoolmodel.h"
#include "cameramodel.h"

ModellingToolModel::ModellingToolModel()
	: m_volume(nullptr) 
{
	// m_scene = new SceneModel;
	// m_cameras[0] = new CameraModel(false, QVector3D(0, 0, -5));
	// m_cameras[1] = new CameraModel(true, QVector3D(0, 0, -5));
	// m_cameras[2] = new CameraModel(true, QVector3D(0, 0, -5), QQuaternion::fromAxisAndAngle(0, 1, 0, 90));
	// m_cameras[3] = new CameraModel(true, QVector3D(0, 0, -5), QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
}

ModellingToolModel::~ModellingToolModel()
{
	//delete m_scene;
	delete m_volume;
	for (int i = 0; i < 4; i++)
	{
		delete m_cameras[i];
	}
}

VolumeModel* ModellingToolModel::getVolume() const
{
	return m_volume;
}

//SceneModel* ModellingToolModel::getScene() const
//{
//	return m_scene;
//}

CameraModel* ModellingToolModel::getCamera(int number) const
{
	return m_cameras[number];
}

void ModellingToolModel::setVolume(VolumeModel* volume)
{
	delete m_volume;
	m_volume = volume;
}
