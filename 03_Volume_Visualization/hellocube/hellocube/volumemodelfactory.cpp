#include "volumemodelfactory.h"
#include <QFileDialog>
#include <fstream>
#include <QOpenGLTexture>

VolumeModel* VolumeModelFactory::createFromFile(const QString& fileName)
{
	auto vec = readFile(fileName);
	//todo loop over data.
	auto texture = new QOpenGLTexture(QOpenGLTexture::Target3D);
	texture->setData(QOpenGLTexture::PixelFormat::Alpha, QOpenGLTexture::PixelType::UInt8, vec.data());
	texture->allocateStorage();
}

std::vector<unsigned char> VolumeModelFactory::readFile(const QString& fileName)
{
	//todo try first reading first two lines and then loop over rest of data.
	auto bytes = fileName.toLatin1();
	const char *fileNameChar = bytes.data();

	std::ifstream file(fileNameChar, std::ios::binary);

	file.unsetf(std::ios::skipws);

	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),	std::istream_iterator<unsigned char>(file),	std::istream_iterator<unsigned char>());
	return vec;
}
