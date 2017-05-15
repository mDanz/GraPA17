#pragma once
#include "scenemodel.h"
#include <QVector3D>


struct CameraModel
{
	QVector3D eye;
	QVector3D up;
	QVector3D center;
	bool m_isOrthographic;


	CameraModel(bool isOrthographic = false);
	~CameraModel();
	QMatrix4x4 GetCameraMatrix() const;
	bool isOrthographic() const;
};

class ModellingToolModel
{
public:
	ModellingToolModel();
	~ModellingToolModel();
	SceneModel* GetScene() const;
	const CameraModel* GetCamera(int number) const;


private:
	SceneModel *m_scene;
	QList<CameraModel> *m_cameras;

};