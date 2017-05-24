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
		auto bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto xDim = atoi(bytes);
		delete[] bytes;

		auto byte = new char;
		file.read(byte, 1);
		delete byte;

		bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto yDim = atoi(bytes);
		delete[] bytes;

		byte = new char;
		file.read(byte, 1);
		delete byte;

		bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto zDim = atoi(bytes);
		delete[] bytes;

		byte = new char;
		file.read(byte, 1);
		delete byte;

		model.setDimensions(xDim, yDim, zDim);

		bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto xAspect = atof(bytes);
		delete[] bytes;

		byte = new char;
		file.read(byte, 1);
		delete byte;

		bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto yAspect = atof(bytes);
		delete[] bytes;

		byte = new char;
		file.read(byte, 1);
		delete byte;

		bytes = new char[4];
		file.read(bytes, 3);
		bytes[3] = '\0';
		auto zAspect = atof(bytes);
		delete[] bytes;

		byte = new char;
		file.read(byte, 1);
		delete byte;

		model.setAspects(xAspect, yAspect, zAspect);
		
		int headerSize = 24;

		int dataSize = file.size() - headerSize;
		char* data = new char[dataSize];
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
