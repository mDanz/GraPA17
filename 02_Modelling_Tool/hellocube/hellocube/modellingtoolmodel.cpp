
#include "modellingtoolmodel.h"
#include <QMatrix4x4>

CameraModel::CameraModel(bool isOrthographic)
{
	m_isOrthographic = isOrthographic;
}

CameraModel::~CameraModel()
{
}

QMatrix4x4 CameraModel::GetCameraMatrix() const
{
	return QMatrix4x4();
}

bool CameraModel::isOrthographic() const
{
	return m_isOrthographic;
}

ModellingToolModel::ModellingToolModel()
{
	m_scene = new SceneModel;
	m_cameras = new QList<CameraModel>;
	m_cameras->append(CameraModel());
	m_cameras->append(CameraModel(true));
	m_cameras->append(CameraModel(true));
	m_cameras->append(CameraModel(true));
}

ModellingToolModel::~ModellingToolModel()
{
	delete m_scene;
	m_cameras->clear();
	delete m_cameras;
}

SceneModel* ModellingToolModel::GetScene() const
{
	return m_scene;
}

const CameraModel* ModellingToolModel::GetCamera(int number) const
{
	return &(m_cameras->at(number));
}

