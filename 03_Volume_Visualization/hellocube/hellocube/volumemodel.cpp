#include "volumemodel.h"
#include <QOpenGLTexture>

VolumeModel::VolumeModel()
	: m_data(nullptr) 
{
	m_dimensions = new QVector3D;
	m_aspects = new QVector3D;
}

VolumeModel::~VolumeModel()
{
	delete m_data;
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

void VolumeModel::setTexture(QOpenGLTexture *data)
{
	m_data = data;
}
