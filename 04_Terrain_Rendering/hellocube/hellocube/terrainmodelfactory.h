#pragma once
#include <qfile.h>
#include <qopenglfunctions_4_4_compatibility.h>


class QString;
class TerrainModel;

class TerrainModelFactory
{
public:
	static TerrainModel* createFromFile(const QString& fileName);

private:
	static void fillTerrainModel(const QString& fileName, TerrainModel& model);
	static void fillMapSize(QFile& file, TerrainModel& model);
	static void fillMaxValue(QFile& file, TerrainModel& model);
	static void fillData(QFile& file, TerrainModel& model);
	static void createTexture(TerrainModel& model);
	static void generateTexture(TerrainModel& model, QOpenGLFunctions_4_4_Compatibility* glFunc);
};
