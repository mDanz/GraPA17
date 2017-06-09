#pragma once
#include "sceneitem.h"

class TerrainModel : public SceneItem
{
public:
	explicit TerrainModel(RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~TerrainModel();
	void setSize(int width, int height);
	void setMaxValue(int maxVal);
private:
	QPointF *m_scale;
	QPoint *m_MapSize;
	int m_maxValue;
	GLuint m_textureName;

};

