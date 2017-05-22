#pragma once
#include "scenemodel.h"


class CameraModel;

class ModellingToolModel
{
public:
	ModellingToolModel();
	~ModellingToolModel();
	SceneModel* getScene() const;
	CameraModel* getCamera(int number) const;


private:
	SceneModel *m_scene;
	CameraModel *m_cameras[4];

};
