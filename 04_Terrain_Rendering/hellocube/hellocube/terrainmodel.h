#pragma once
#include "sceneitem.h"

class TerrainModel : public SceneItem
{
public:
	explicit TerrainModel(RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~TerrainModel();

private:
	QPointF *m_scale;
	GLuint m_textureName;

};

