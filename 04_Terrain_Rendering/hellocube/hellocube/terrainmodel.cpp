
#include "terrainmodel.h"
#include <QOpenGLTexture>

TerrainModel::TerrainModel(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: m_mapSize(nullptr)
	, m_scalarByteSize(0)
	, m_maxValue(0)
	, m_textureName(0)
	, m_materials(nullptr)
{
	m_scale = new QPointF(1, 1);
}

TerrainModel::~TerrainModel()
{
	delete m_scale;
	delete m_mapSize;
	delete m_materials;
}

int TerrainModel::getHeightScale() const
{
	return m_scale->y();
}

void TerrainModel::setHeightScale(int height)
{
	m_scale->setY(height);
	emit terrainModelChanged();
}

int TerrainModel::getWidthScale() const
{
	return m_scale->x();
}

void TerrainModel::setWidthScale(int width)
{
	m_scale->setX(width);
	emit terrainModelChanged();
}

void TerrainModel::setSize(int width, int height)
{
	if (m_mapSize == nullptr)
	{
		m_mapSize = new QPoint;
	}

	m_mapSize->setX(width);
	m_mapSize->setY(height);

	emit terrainModelChanged();
}

void TerrainModel::setMaxValue(int maxVal)
{
	m_maxValue = maxVal;

	emit terrainModelChanged();
}

void TerrainModel::setData(QByteArray data)
{
	m_data = data;

	if (getScalarType() != GL_UNSIGNED_BYTE)
	{
		fixByteOrder();
	}

	m_floatData = getHeightValues(data);

	emit terrainModelChanged();
}

unsigned char* TerrainModel::getData()
{
	return reinterpret_cast<unsigned char*>(m_data.data());
}

float TerrainModel::getHeightValue(QPoint& pos) const
{
	return m_floatData[pos.x()*m_mapSize->x() + pos.y()];
}

int TerrainModel::getDataSize() const
{
	return m_data.count();
}

int TerrainModel::getHeighMapTextureName() const
{
	return m_textureName;
}

void TerrainModel::setTextureName(int textureName)
{
	m_textureName = textureName;

	emit terrainModelChanged();
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

void TerrainModel::setMaterials(QOpenGLTexture* materials)
{
	m_materials = materials;

	emit terrainModelChanged();
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

float* TerrainModel::getHeightValues(QByteArray &data) const
{
	auto floatData = new float[m_data.size()/m_scalarByteSize];
	auto chars = reinterpret_cast<unsigned char*>(data.data());
	
	for (int i = 0; i < m_data.size(); i += m_scalarByteSize)
	{
		floatData[i / m_scalarByteSize] = getHeightValue(&chars[i]); //todo test this
	}

	return floatData;
}

float TerrainModel::getHeightValue(unsigned char* data) const
{
	switch (m_scalarByteSize)
	{
	case 1:
		return static_cast<unsigned char>(data[0]) / static_cast<float>(0xff);
	case 2:
		return static_cast<unsigned short>(data[1] << 8 | data[0]) / static_cast<float>(0xffff);
	case 4:
		return static_cast<unsigned int>(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / static_cast<float>(0xffffffff);
	default:
		return 0.0f;
	}
}
