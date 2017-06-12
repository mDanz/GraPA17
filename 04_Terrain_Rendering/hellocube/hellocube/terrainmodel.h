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
	float getHeightValue(QPoint &pos) const;
	int getDataSize() const;
	int getHeighMapTextureName() const;
	void setTextureName(int textureName);
	QPoint* getMapSize() const;
	GLuint getScalarType();
	void setMaterials(QVector<GLuint> *materialFiles);
	QVector<GLuint> *getMaterials();
	bool isWireframeEnabled() const;
	void updateWireframEnabled(bool isEnabled);
	bool isReady() const;
	void setReady(bool flag);

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
	QVector<GLuint> *m_materials;
	bool m_isWireframeEnabled;
	bool m_isReady;
};

