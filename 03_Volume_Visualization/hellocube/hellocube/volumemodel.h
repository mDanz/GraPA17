#pragma once
#include <QVector3D>
#include <QOpenGLFunctions_4_4_Compatibility>
#include "sceneitem.h"


class QVector3D;
class QOpenGLTexture;

class VolumeModel : public SceneItem
{
public:
	explicit VolumeModel(RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~VolumeModel();

	void setDimensions(float x, float y, float z) const;
	void setAspects(float x, float y, float z) const;
	float getMinValue() const;
	void setMinValue(float value);
	float getMaxValue() const;
	void setMaxValue(float value);

	void setDataTexture(QOpenGLTexture *data);
	void setData(QByteArray data);
	QOpenGLTexture* getDataTexture() const;
	unsigned char* getData();
	QByteArray* getByteArrayData();
	int getDataSize() const;
	GLuint getScalarType();
	QVector3D* getAspects() const;
	QVector3D* getDimensions() const;
	int getScalarByteSize() const;
	int getTextureName() const;
	void setTextureName(int tex_name);
	QMatrix4x4 getLocalMatrix() const;
	QMatrix4x4 getNormalizationMatrix() const;
private:
	QVector3D *m_dimensions;
	QVector3D *m_aspects;
	float m_minValue;
	float m_maxValue;
	QByteArray m_data;
	int m_scalarByteSize;
	QOpenGLTexture *m_dataTexture;
	int m_textureName;
};
