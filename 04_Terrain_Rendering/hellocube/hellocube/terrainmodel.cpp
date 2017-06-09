
#include "terrainmodel.h"

TerrainModel::TerrainModel(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: m_scale(nullptr)
	, m_MapSize(nullptr)
	, m_maxValue(0)
	, m_textureName(0)
{
}

TerrainModel::~TerrainModel()
{
	delete m_scale;
	delete m_MapSize;
}

void TerrainModel::setSize(int width, int height)
{
	if (m_MapSize == nullptr)
	{
		m_MapSize = new QPoint;
	}

	m_MapSize->setX(width);
	m_MapSize->setY(height);
}

void TerrainModel::setMaxValue(int maxVal)
{
	m_maxValue = maxVal;
}
