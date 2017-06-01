#include "volumemodelfactory.h"
#include <QFileDialog>
#include <fstream>
#include <QOpenGLTexture>
#include "volumemodel.h"
#include "openglhelper.h"
#include <QByteArray>

VolumeModel* VolumeModelFactory::createFromFile(const QString& fileName)
{
	auto model = new VolumeModel;
	fillVolumeModel(fileName, *model);//todo loop over data.
	
	return model;
}

void VolumeModelFactory::fillVolumeModel(const QString& fileName, VolumeModel &model)
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

	fillDimensions(model, file);
	fillAspects(model, file);
	fillData(model, file);
	fillTexture(model);

	qInfo() << "volume data errors: " << OpenGLHelper::Error();
}

void VolumeModelFactory::fillDimensions(VolumeModel &model, QFile &file)
{
	auto line = file.readLine();
	QTextStream readStream(line);
	int xDim, yDim, zDim;
	readStream >> xDim >> yDim >> zDim;
	model.setDimensions(xDim, yDim, zDim);
}

void VolumeModelFactory::fillAspects(VolumeModel &model, QFile &file)
{
	auto line = file.readLine();
	QTextStream readStream(line);
	float xAspect, yAspect, zAspect;
	readStream >> xAspect >> yAspect >> zAspect;
	model.setAspects(xAspect, yAspect, zAspect);
}

void VolumeModelFactory::fillData(VolumeModel& model, QFile& file)
{
	auto bytes = file.readAll();
	model.setData(bytes);
}

void VolumeModelFactory::fillTexture(VolumeModel &model)
{
	auto glFunc = OpenGLHelper::getGLFunc();
	generateTexture(model, glFunc);
	glFunc->glBindTexture(GL_TEXTURE_3D, model.getTextureName());

	// set up wrapping, filtering and pixel alignment
	glFunc->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glFunc->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFunc->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glFunc->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFunc->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFunc->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	model.setHistogram(new Histogram(model.getByteArrayData(), model.getScalarByteSize(), 256));
	qInfo() << "Intensity values between " << model.getMinValue() << "and" << model.getMaxValue() << " and scalar Type: " << model.getScalarType() << "Size: " <<model.getScalarByteSize();

	glFunc->glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, model.getDimensions()->x(), model.getDimensions()->y(), model.getDimensions()->z(), 0, GL_RED, model.getScalarType(), model.getData());
	glFunc->glBindTexture(GL_TEXTURE_3D, model.getTextureName());
}


void VolumeModelFactory::generateTexture(VolumeModel& model, QOpenGLFunctions_4_4_Compatibility* glFunc)
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
		qInfo() << "Volume texture" << model.getTextureName() << "created.";
	}
}
