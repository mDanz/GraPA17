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

	//emit dataChanged(); todo emit data change adn catch somewhere
}

void VolumeModelFactory::skipByte(QFile &file)
{
	auto byte = new char;
	file.read(byte, 1);
	delete byte;
}

int VolumeModelFactory::readInt(QFile &file)
{
	auto bytes = new char[4];
	file.read(bytes, 3);
	bytes[3] = '\0';
	auto i = atoi(bytes);
	delete[] bytes;
	return i;
}

float VolumeModelFactory::readFloat(QFile &file)
{
	auto bytes = new char[4];
	file.read(bytes, 3);
	bytes[3] = '\0';
	auto f = atof(bytes);
	delete[] bytes;
	return f;
}

void VolumeModelFactory::fillDimensions(VolumeModel &model, QFile &file)
{
	auto xDim = readInt(file);
	skipByte(file);
	auto yDim = readInt(file);
	skipByte(file);
	auto zDim = readInt(file);
	skipByte(file);
	model.setDimensions(xDim, yDim, zDim);
}

void VolumeModelFactory::fillAspects(VolumeModel &model, QFile &file)
{
	auto xAspect = readFloat(file);
	skipByte(file);
	auto yAspect = readFloat(file);
	skipByte(file);
	auto zAspect = readFloat(file);
	skipByte(file);
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

	auto dataType = model.getScalarType();

	//// fix the byte order to big endian and find max and min values todo fix byteorder
	auto d = model.getData();
	auto scalarByteSize = model.getScalarByteSize();
	int domain = pow(256, scalarByteSize);
	int l, r, maxV = 0, minV = domain, v;
	char tmp;

	//for (int i = 0; i < model.getDataSize(); i += scalarByteSize) {
	//	for (int n = 0; n < (scalarByteSize + 1) / 2; n++) {
	//		l = i + n;
	//		r = i + scalarByteSize - 1 - n;
	//		tmp = model.getByteArrayData()->at(l);
	//		model.getByteArrayData()[l] = model.getByteArrayData()[r];
	//		model.getByteArrayData()->insert(r, tmp);
	//	}

	//	// determine current Value
	//	v = 0;
	//	for (int b = scalarByteSize - 1; b >= 0; b--)
	//	{
	//		v = (v << 8) | static_cast<int>(d[i + b]);
	//	}

	//	if (v > maxV)
	//	{
	//		maxV = v;
	//	}
	//	if (v < minV)
	//	{
	//		minV = v;
	//	}
	//}
	// normalize the max and min intensity values

	for (int i = 0; i < model.getDataSize(); i++)
	{
		if (d[i] > maxV)
		{
			maxV = d[i];
		}
		if (d[i] < minV)
		{
			minV = d[i];
		}

	}
	model.setMinValue(minV / (float)domain);
	model.setMaxValue(maxV / (float)domain);
	qInfo() << "Intensity values between " << model.getMinValue() << "and" << model.getMaxValue();

	glFunc->glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, model.getDimensions()->x(), model.getDimensions()->y(), model.getDimensions()->z(), 0, GL_RED, dataType, model.getData());
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
