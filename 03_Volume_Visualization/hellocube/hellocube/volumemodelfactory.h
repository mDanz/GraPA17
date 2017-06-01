#pragma once
#include <qfile.h>
#include <QOpenGLFunctions_4_4_Compatibility>


class QString;
class VolumeModel;

class VolumeModelFactory
{
public:
	static VolumeModel* createFromFile(const QString& fileName);
private:
	static void fillVolumeModel(const QString& fileName, VolumeModel &model);
	static void fillDimensions(VolumeModel &model, QFile &file);
	static void fillAspects(VolumeModel &model, QFile &file);
	static void fillData(VolumeModel& model, QFile& file);
	static void fillTexture(VolumeModel &model);
	static void generateTexture(VolumeModel& model, QOpenGLFunctions_4_4_Compatibility* glFunc);
};

