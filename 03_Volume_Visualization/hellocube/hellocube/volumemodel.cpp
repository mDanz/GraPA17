#include "volumemodel.h"
#include "openglhelper.h"
#include <QOpenGLTexture>

VolumeModel::VolumeModel(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent)
	: SceneItem(OpenGLPrimitiveType::Volume, rigidBodyTransform, parent)
	, m_data(nullptr)
{
	m_dimensions = new QVector3D;
	m_aspects = new QVector3D;
	m_textureName = GL_INVALID_VALUE;
}

VolumeModel::~VolumeModel()
{
	delete m_dimensions;
	delete m_aspects;
}

void VolumeModel::setDimensions(float x, float y, float z) const
{
	m_dimensions->setX(x);
	m_dimensions->setY(y);
	m_dimensions->setZ(z);
}

void VolumeModel::setAspects(float x, float y, float z) const
{
	m_aspects->setX(x);
	m_aspects->setY(y);
	m_aspects->setZ(z);
}

float VolumeModel::getMinValue() const
{
	return m_minValue;
}

void VolumeModel::setMinValue(float value)
{
	m_minValue = value;
}

float VolumeModel::getMaxValue() const
{
	return m_maxValue;
}

void VolumeModel::setMaxValue(float value)
{
	m_maxValue = value;
}

void VolumeModel::setDataTexture(QOpenGLTexture *dataTexture)
{
	m_dataTexture = dataTexture;
}

void VolumeModel::setData(QByteArray data)
{
	m_data = data;
}

QOpenGLTexture* VolumeModel::getDataTexture() const
{
	return m_dataTexture;
}

unsigned char* VolumeModel::getData()
{
	return reinterpret_cast<unsigned char*>(m_data.data());
}

QByteArray* VolumeModel::getByteArrayData()
{
	return &m_data;
}

int VolumeModel::getDataSize() const
{
	return m_data.size();
	/*auto size = sizeof(m_data) / sizeof(m_data[0]);
	return size;*/
}

GLuint VolumeModel::getScalarType()
{
	int bytesPerScalar = getDataSize() / (m_dimensions->x()*m_dimensions->y()*m_dimensions->z());
	
	switch (bytesPerScalar) {
	case 1:
		m_scalarByteSize = 1;
		return GL_UNSIGNED_BYTE;
	case 2:
		m_scalarByteSize = 2;
		return GL_UNSIGNED_SHORT;
	case 4:
		m_scalarByteSize = 4;
		return GL_UNSIGNED_INT;
	default:
		qWarning() << "Unsupported data type! " << bytesPerScalar << "bytes per value. Using default type unsigned byte.";
		m_scalarByteSize = 1;
		return GL_UNSIGNED_BYTE;
	}
}

QVector3D* VolumeModel::getAspects() const
{
	return m_aspects;
}

QVector3D* VolumeModel::getDimensions() const
{
	return m_dimensions;
}

int VolumeModel::getScalarByteSize() const
{
	return m_scalarByteSize;
}

int VolumeModel::getTextureName() const
{
	return m_textureName;
}

void VolumeModel::setTextureName(int textureName)
{
	m_textureName = textureName;
}

QMatrix4x4 VolumeModel::getLocalMatrix() const
{
	auto mat = getRigidBodyTransformation()->getWorldMatrix();
	return mat * getNormalizationMatrix();
}

QMatrix4x4 VolumeModel::getNormalizationMatrix() const
{
	float width = m_dimensions->x() * m_aspects->x();
	float height = m_dimensions->y() * m_aspects->y();
	float depth = m_dimensions->z() * m_aspects->z();
	float max;
	max = (width > height) 
		? ((width > depth) ? width : depth) 
		: ((height > depth) ? height : depth);

	QMatrix4x4 m;
	m.scale(width / max, height / max, depth / max);
	return m;
}
