
#include "terrainmodel.h"
#include <QOpenGLTexture>

TerrainModel::TerrainModel(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: SceneItem(Terrain, rigidBodyTransform, parent)
	, m_mapSize(nullptr)
	, m_floatData(nullptr)
	, m_scalarByteSize(0)
	, m_maxValue(0)
	, m_textureName(GL_INVALID_VALUE)
	, m_materials(nullptr)
	, m_isWireframeEnabled(false)
	, m_isReady(false)
{
	m_scale = new QPointF(128, 100);
	m_fallOff = 1500.f;
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
	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

int TerrainModel::getWidthScale() const
{
	return m_scale->x();
}

void TerrainModel::setWidthScale(int width)
{
	m_scale->setX(width);
	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

void TerrainModel::setSize(int width, int height)
{
	if (m_mapSize == nullptr)
	{
		m_mapSize = new QPoint;
	}

	m_mapSize->setX(width);
	m_mapSize->setY(height);

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

void TerrainModel::setMaxValue(int maxVal)
{
	m_maxValue = maxVal;

	if (m_isReady)
	{
		emit terrainModelChanged();
	};
}

void TerrainModel::setData(QByteArray data)
{
	m_data = data;

	//if (getScalarType() != GL_UNSIGNED_BYTE)
	{
		getScalarType();
		fixByteOrderAndFillHeightValues();
	}

	//m_floatData = getHeightValues(data);

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

QVector<unsigned short> TerrainModel::getData() const
{
	return m_realData;
}

float TerrainModel::getHeightValue(QPoint& pos) const
{
	return m_realData.at(pos.x()*m_mapSize->x() + pos.y());
}

int TerrainModel::getDataSize() const
{
	return m_data.size();
}

int TerrainModel::getHeighMapTextureName() const
{
	return m_textureName;
}

void TerrainModel::setTextureName(int textureName)
{
	m_textureName = textureName;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
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

void TerrainModel::setMaterials(QVector<GLuint> *materialFiles)
{
	m_materials = materialFiles;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

QVector<GLuint>* TerrainModel::getMaterials()
{
	if (!m_materials)
	{
		m_materials = new QVector<GLuint>;
	}
	return m_materials;
}

bool TerrainModel::isWireframeEnabled() const
{
	return m_isWireframeEnabled;
}

void TerrainModel::updateWireframEnabled(bool isEnabled)
{
	m_isWireframeEnabled = isEnabled;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

bool TerrainModel::isReady() const
{
	return m_isReady;
}

void TerrainModel::setReady(bool flag)
{
	m_isReady = flag;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

void TerrainModel::setHeighMapImage(QImage& img)
{
	m_heightMapImage = img;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

QImage TerrainModel::getHeightMapImage() const
{
	return m_heightMapImage;
}

void TerrainModel::setFallOff(float falloff)
{
	m_fallOff = falloff;

	if (m_isReady)
	{
		emit terrainModelChanged();
	}
}

float TerrainModel::getFallOff() const
{
	return m_fallOff;
}

void TerrainModel::fixByteOrderAndFillHeightValues()
{
	char tmp; //todo refactor
	int left;
	int right;
	//m_realData = new QVector<ushort>;//[m_data.size() / 2];
	auto data = reinterpret_cast<uchar*>(m_data.data());
	int v;
	int maxV = 0;
	int domain = pow(256, m_scalarByteSize);
	for (int i = 0; i < m_data.size(); i += m_scalarByteSize)
	{
		for (int n = 0; n < (m_scalarByteSize + 1) / 2; n++)
		{
			left = i + n;
			right = i + m_scalarByteSize - 1 - n;
			tmp = m_data[left];
			m_data[left] = m_data[right];
			m_data[right] = tmp;
		}

		v = 0;
		for (int b = m_scalarByteSize - 1; b >= 0; b--) 
		{
			v = (v << 8) | static_cast<int>(data[i + b]);
		}

		//m_realData[i / 2] = v;
		m_realData.append(v);

		if (v > maxV) 
		{
			maxV = v;
		}
	}

	auto scaling = static_cast<float>(domain) / static_cast<float>(maxV);
	for (int i = 0; i < m_data.size() / 2; i++) 
	{
		m_realData[i] *= scaling;
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
