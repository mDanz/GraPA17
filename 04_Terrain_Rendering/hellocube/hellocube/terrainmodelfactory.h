#pragma once
#include <qfile.h>


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
};
