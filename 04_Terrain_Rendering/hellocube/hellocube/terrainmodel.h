#pragma once
#include "sceneitem.h"

class TerrainModel : public SceneItem
{
public:
	explicit TerrainModel(RigidBodyTransformation *rigidBodyTransform = nullptr, SceneItem *parent = nullptr);
	~TerrainModel();
	void setSize(int width, int height);
	void setMaxValue(int maxVal);
	void setData(QByteArray data);
	unsigned char* getData();
	int getDataSize() const;
	int getTextureName() const;
	void setTextureName(int textureName);
	QPoint* getMapSize() const;
	GLuint getScalarType();

private:
	void fixByteOrder();

	QPointF *m_scale;
	QPoint *m_mapSize;
	QByteArray m_data;
	int m_scalarByteSize;
	int m_maxValue;
	GLuint m_textureName;

};

