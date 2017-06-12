#pragma once
#include <qfile.h>
#include <qopenglfunctions_4_4_compatibility.h>


class QString;
class TerrainModel;

class TerrainModelFactory
{
public:
	static TerrainModel* createFromFile(const QString& heightMapFile, const QStringList& textureFiles);

private:
	static void fillTerrainModel(const QString& hightMapFile, const QStringList& textureFiles, TerrainModel& model);
	static void fillMapSize(QFile& file, TerrainModel& model);
	static void fillMaxValue(QFile& file, TerrainModel& model);
	static void fillData(QFile& file, TerrainModel& model);
	static void createHeightMapTexture(TerrainModel& model);
	static void generateHeightMapTexture(TerrainModel& model, QOpenGLFunctions_4_4_Compatibility* glFunc);
	static void createMaterialTextures(const QStringList& textureFiles, TerrainModel& model);
	static GLuint createMaterial(const QString& path);
};
