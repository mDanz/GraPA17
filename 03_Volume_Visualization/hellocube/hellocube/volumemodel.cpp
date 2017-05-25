#include "volumemodel.h"
#include "openglhelper.h"
#include <QOpenGLTexture>

VolumeModel::VolumeModel()
	: m_data(nullptr) 
{
	m_dimensions = new QVector3D;
	m_aspects = new QVector3D;
	m_textureName = GL_INVALID_VALUE;
}

VolumeModel::VolumeModel(OpenGLGeometryType primitiveType, RigidBodyTransformation& rigidBodyTransform, SceneItem* parent)
	: SceneItem(primitiveType, rigidBodyTransform, parent)
	, m_data(nullptr)
{
	m_dimensions = new QVector3D;
	m_aspects = new QVector3D;
	m_textureName = GL_INVALID_VALUE;
}

VolumeModel::~VolumeModel()
{
	delete[] m_data;
	delete m_dimensions;
	delete m_aspects;
}

void VolumeModel::setDimensions(float x, float y, float z)
{
	m_dimensions->setX(x);
	m_dimensions->setY(y);
	m_dimensions->setZ(z);
}

void VolumeModel::setAspects(float x, float y, float z)
{
	m_aspects->setX(x);
	m_aspects->setY(y);
	m_aspects->setZ(z);
}

void VolumeModel::setData(QOpenGLTexture *dataTexture)
{
	m_dataTexture = dataTexture;
}

void VolumeModel::setData(unsigned char* data)
{
	m_data = data;
}

QOpenGLTexture* VolumeModel::getDataTexture() const
{
	return m_dataTexture;
}

unsigned char* VolumeModel::getData() const
{
	return m_data;
}

int VolumeModel::getDataSize() const
{
	return sizeof(m_data) / sizeof(m_data[0]);
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
