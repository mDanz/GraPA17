
#include "terrainmodel.h"

TerrainModel::TerrainModel(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: m_scale(nullptr)
	, m_mapSize(nullptr)
	, m_maxValue(0)
	, m_textureName(0)
{
}

TerrainModel::~TerrainModel()
{
	delete m_scale;
	delete m_mapSize;
}

void TerrainModel::setSize(int width, int height)
{
	if (m_mapSize == nullptr)
	{
		m_mapSize = new QPoint;
	}

	m_mapSize->setX(width);
	m_mapSize->setY(height);
}

void TerrainModel::setMaxValue(int maxVal)
{
	m_maxValue = maxVal;
}

void TerrainModel::setData(QByteArray data)
{
	m_data = data;
	if (getScalarType() != GL_UNSIGNED_BYTE)
	{
		fixByteOrder();
	}
}

unsigned char* TerrainModel::getData()
{
	return reinterpret_cast<unsigned char*>(m_data.data());
}

int TerrainModel::getDataSize() const
{
	return m_data.count();
}

int TerrainModel::getTextureName() const
{
	return m_textureName;
}

void TerrainModel::setTextureName(int textureName)
{
	m_textureName = textureName;
}

QPoint* TerrainModel::getMapSize() const
{
	return m_mapSize;
}

GLuint TerrainModel::getScalarType()
{
	if (m_maxValue < 256 && m_maxValue > 0)
	{
		m_scalarByteSize = 1;
		return GL_UNSIGNED_BYTE;
	}
	else if (m_maxValue < 65536 && m_maxValue >= 256)
	{
		m_scalarByteSize = 2;
		return GL_UNSIGNED_SHORT;
	}
	qWarning() << "Unsupported data type! Max Value: " << m_maxValue << " is not supported. Using default type unsigned byte.";
	m_scalarByteSize = 1;
	return GL_UNSIGNED_BYTE;
}

void TerrainModel::fixByteOrder()
{
	char tmp;
	int left;
	int right;
	for (int i = 0; i < getDataSize(); i += m_scalarByteSize)
	{
		for (int n = 0; n < (m_scalarByteSize + 1) / 2; n++)
		{
			left = i + n;
			right = i + m_scalarByteSize - 1 - n;
			tmp = m_data[left];
			m_data[left] = m_data[right];
			m_data[right] = tmp;
		}
	}
}
