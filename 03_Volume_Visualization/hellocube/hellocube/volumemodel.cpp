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
	m_transferFunction = new TransferFunction();
}

VolumeModel::~VolumeModel()
{
	OpenGLHelper::getGLFunc()->glDeleteTextures(1, &m_textureName);
	delete m_transferFunction;
	delete m_histogram;
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
	return m_histogram->getMinValue();
}

float VolumeModel::getMaxValue() const
{
	return m_histogram->getMaxValue();
}

void VolumeModel::setData(QByteArray data)
{
	m_data = data;
	if (getScalarType() != GL_UNSIGNED_BYTE)
	{
		fixByteOrder();
	}
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

void VolumeModel::setHistogram(Histogram *histogram)
{
	m_histogram = histogram;
}

Histogram* VolumeModel::getHistogram() const
{
	return m_histogram;
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

	QMatrix4x4 mat;
	mat.scale(width / max, height / max, depth / max);
	return mat;
}

TransferFunction* VolumeModel::getTransferFunction() const
{
	return m_transferFunction;
}

void VolumeModel::setTransferFunction(TransferFunction* transferFunction)
{
	m_transferFunction = transferFunction;
}

void VolumeModel::fixByteOrder()
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
