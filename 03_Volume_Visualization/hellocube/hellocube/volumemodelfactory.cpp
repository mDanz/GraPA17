#include "volumemodelfactory.h"
#include <QFileDialog>
#include <fstream>
#include <QOpenGLTexture>
#include "volumemodel.h"

VolumeModel* VolumeModelFactory::createFromFile(const QString& fileName)
{
	auto model = new VolumeModel;
	fillVolumeModel(fileName, *model);//todo loop over data.
	
	return model;
}

void VolumeModelFactory::fillVolumeModel(const QString& fileName, VolumeModel &model)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) { return; }

	fillDimensions(model, file);
	fillAspects(model, file);
	fillTexture(model, file);
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

void VolumeModelFactory::fillTexture(VolumeModel &model, QFile &file)
{
	int headerSize = 24;
	int dataSize = file.size() - headerSize;
	auto data = new char[dataSize];
	file.read(data, dataSize);

	unsigned char* byteData;
	byteData = reinterpret_cast<unsigned char*>(data);
	data = nullptr;

	auto texture = new QOpenGLTexture(QOpenGLTexture::Target3D);
	texture->setData(QOpenGLTexture::PixelFormat::Alpha, QOpenGLTexture::PixelType::UInt8, byteData);
	texture->allocateStorage();
		
	delete[] byteData;
	model.setTexture(texture);
}
