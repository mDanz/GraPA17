#pragma once
#include "sceneitem.h"
#include "histogram.h"
#include "transferfunction.h"


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
	float getMaxValue() const;

	void setData(QByteArray data);
	unsigned char* getData();
	QByteArray* getByteArrayData();
	int getDataSize() const;
	GLuint getScalarType();
	QVector3D* getAspects() const;
	QVector3D* getDimensions() const;
	int getScalarByteSize() const;
	void setHistogram(Histogram *histogram);
	Histogram* getHistogram() const;
	int getTextureName() const;
	void setTextureName(int tex_name);
	QMatrix4x4 getLocalMatrix() const;
	QMatrix4x4 getNormalizationMatrix() const;
	TransferFunction* getTransferFunction() const;
	void setTransferFunction(TransferFunction *transferFunction);
private:
	void fixByteOrder();

	QVector3D *m_dimensions;
	QVector3D *m_aspects;
	float m_minValue;
	float m_maxValue;
	QByteArray m_data;
	int m_scalarByteSize;
	GLuint m_textureName;

	Histogram *m_histogram;
	TransferFunction *m_transferFunction;
};
