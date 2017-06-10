#pragma once
#include "sceneitem.h"

class QOpenGLTexture;

class TerrainModel : public SceneItem
{
	Q_OBJECT

public:
	explicit TerrainModel(RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~TerrainModel();

	int getHeightScale() const;
	void setHeightScale(int height);
	int getWidthScale() const;
	void setWidthScale(int width);
	void setSize(int width, int height);
	void setMaxValue(int maxVal);
	void setData(QByteArray data);
	unsigned char* getData();
	float getHeightValue(QPoint &pos);
	int getDataSize() const;
	int getHeighMapTextureName() const;
	void setTextureName(int textureName);
	QPoint* getMapSize() const;
	GLuint getScalarType();
	void setMaterials(QOpenGLTexture* materials);

signals:
	void terrainModelChanged();

private:
	void fixByteOrder();
	float* getHeightValues(QByteArray &data) const;
	float getHeightValue(unsigned char* data) const;

	QPointF *m_scale;
	QPoint *m_mapSize;
	QByteArray m_data;
	float *m_floatData;
	int m_scalarByteSize;
	int m_maxValue;
	GLuint m_textureName;
	QOpenGLTexture* m_materials;
};

