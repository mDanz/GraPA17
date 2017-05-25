#pragma once
#include "scenemodel.h"
#include "cameramodel.h"


class ViewPortModel
{
public:
	explicit ViewPortModel(SceneModel *scene);
	~ViewPortModel();

	SceneModel* getScene() const;
	CameraModel* getCamera(int number) const;

private:
	SceneModel *m_scene;
	CameraModel *m_cameras[4];
};
