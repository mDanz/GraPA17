
#include "terrainmodelfactory.h"
#include "terrainmodel.h"
#include <QFileDialog>
#include <QOpenGLFunctions_4_4_Compatibility>
#include "openglhelper.h"

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
	fillData(file, model);
	createTexture(model);

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

void TerrainModelFactory::fillData(QFile& file, TerrainModel& model)
{
	auto bytes = file.readAll();
	model.setData(bytes);
}

void TerrainModelFactory::createTexture(TerrainModel& model)
{
	auto glFunc = OpenGLHelper::getGLFunc();
	generateTexture(model, glFunc);
	glFunc->glBindTexture(GL_TEXTURE_2D, model.getTextureName());

	// set up wrapping, filtering and pixel alignment todo see if this is correct
	glFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glFunc->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);
	glFunc->glGenerateMipmap(GL_TEXTURE_2D);
	//glFunc->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//model.setHistogram(new Histogram(model.getByteArrayData(), model.getScalarByteSize(), 256));
	//qInfo() << "Intensity values between " << model.getMinValue() << "and" << model.getMaxValue() << " and scalar Type: " << model.getScalarType() << "Size: " << model.getScalarByteSize();

	glFunc->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, model.getMapSize()->x(), model.getMapSize()->y(), 0, GL_RGBA, model.getScalarType(), model.getData());
	glFunc->glBindTexture(GL_TEXTURE_2D, model.getTextureName());
}

void TerrainModelFactory::generateTexture(TerrainModel& model, QOpenGLFunctions_4_4_Compatibility* glFunc)
{
	glFunc->glActiveTexture(GL_TEXTURE0);
	if (model.getTextureName() == GL_INVALID_VALUE)
	{
		GLuint texName;
		glFunc->glGenTextures(1, &texName);
		model.setTextureName(texName);
	}

	if (model.getTextureName() == GL_INVALID_VALUE)
	{
		qInfo() << "Texture name invalid!";
	}
	else
	{
		qInfo() << "Terrain hight map texture" << model.getTextureName() << "created.";
	}
}
