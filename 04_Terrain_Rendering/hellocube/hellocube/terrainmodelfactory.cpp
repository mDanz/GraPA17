
#include "terrainmodelfactory.h"
#include "terrainmodel.h"
#include <QFileDialog>

TerrainModel* TerrainModelFactory::createFromFile(const QString& fileName)
{
	auto model = new TerrainModel;
	fillTerrainModel(fileName, *model);

	return model;
}

void TerrainModelFactory::fillTerrainModel(const QString& fileName, TerrainModel& model)
{
	QFile file(fileName);
	if (!file.exists()) {
		qWarning() << "File '" << fileName << "'' does not exist!";
		return;
	}

	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Could not open file '" << fileName << "'' !";
		return;
	}

	auto line = file.readLine();
	if (line != "P5")
	{
		qWarning() << "Unsupported file format: " << line << "!";
		return;
	}

	fillMapSize(file, model);
	fillMaxValue(file, model);

}

void TerrainModelFactory::fillMapSize(QFile& file, TerrainModel& model)
{
	int width = 0;
	int height = 0;
	auto lines = file.readLine();
	lines += file.readLine();
	QTextStream readStream(lines);
	readStream >> width >> height;
	model.setSize(width, height);
}

void TerrainModelFactory::fillMaxValue(QFile& file, TerrainModel& model)
{
	int maxVal = 0;
	auto line = file.readLine();
	QTextStream readStream(line);
	readStream >> maxVal;
	model.setMaxValue(maxVal);
}
