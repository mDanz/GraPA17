#pragma once
#include "scenemodel.h"
#include "volumemodel.h"


class CameraModel;

class ModellingToolModel
{
public:
	ModellingToolModel();
	~ModellingToolModel();
	//SceneModel* getScene() const;
	VolumeModel* getVolume() const;
	CameraModel* getCamera(int number) const;
	void setVolume(VolumeModel *volume);

private:
	VolumeModel *m_volume;
	//SceneModel *m_scene;
	CameraModel *m_cameras[4];

};
