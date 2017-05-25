#pragma once
#include <QVector3D>
#include <QOpenGLFunctions_4_4_Compatibility>


class QVector3D;
class QOpenGLTexture;

class VolumeModel
{
public:
	VolumeModel();
	~VolumeModel();

	void setDimensions(float x, float y, float z);
	void setAspects(float x, float y, float z);
	void setData(QOpenGLTexture *data);
	void setData(unsigned char *data);
	QOpenGLTexture* getDataTexture() const;
	unsigned char* getData() const;
	int getDataSize() const;
	GLuint getScalarType();
	QVector3D* getAspects() const;
	QVector3D* getDimensions() const;
	int getScalarByteSize() const;
	int getTextureName() const;
	void setTextureName(int tex_name);
private:
	QVector3D *m_dimensions;
	QVector3D *m_aspects;
	unsigned char *m_data;
	int m_scalarByteSize;
	QOpenGLTexture *m_dataTexture;
	int m_textureName;
};
