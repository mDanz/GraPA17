#pragma once
#include "openglgeometry.h"
#include <QOpenGLBuffer>


class TerrainModel;
class CameraModel;

class OpenGLTerrain : public OpenGLGeometry
{
public:
	OpenGLTerrain();
	~OpenGLTerrain();

	virtual void draw(QOpenGLShaderProgram *program) override;
	void draw(TerrainModel &terrain, CameraModel &camera);

private:
	void initializeShaderProgram();

	void initCubeGeometry();


	const QString m_terrain_vshFile = "./Resources/shaders/terrain.vsh";
	const QString m_terrain_tcsFile = "./Resources/shaders/terrain.tcs";
	const QString m_terrain_tesFile = "./Resources/shaders/terrain.tes";
	const QString m_terrain_fshFile = "./Resources/shaders/terrain.fsh";
	const float m_step = 1.f;
	const int m_gridSize = 50; //needs to be even number
	const float m_terrainScaling = 128.f;

	QOpenGLShaderProgram* m_terrainProgram;
	QOpenGLBuffer m_vertexBuf;
	QOpenGLBuffer m_indexBuf;

};

