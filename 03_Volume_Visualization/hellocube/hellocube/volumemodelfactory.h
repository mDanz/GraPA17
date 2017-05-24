#pragma once
#include <memory>
#include <vector>
#include <qfile.h>


class QString;
class VolumeModel;

class VolumeModelFactory
{
public:
	static VolumeModel* createFromFile(const QString& fileName);
private:
	static void fillVolumeModel(const QString& fileName, VolumeModel &model);
	static void skipByte(QFile &file);
	static int readInt(QFile &file);
	static float readFloat(QFile &file);
	static void fillDimensions(VolumeModel &model, QFile &file);
	static void fillAspects(VolumeModel &model, QFile &file);
	static void fillTexture(VolumeModel &model, QFile &file);
};

