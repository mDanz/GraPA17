#pragma once
#include <QVector3D>


class QVector3D;
class QOpenGLTexture;

class VolumeModel
{
public:
	VolumeModel();
	~VolumeModel();

	void setDimensions(float x, float y, float z);
	void setAspects(float x, float y, float z);
	void setTexture(QOpenGLTexture *data);

private:
	QVector3D *m_dimensions;
	QVector3D *m_aspects;
	QOpenGLTexture *m_data;
};
