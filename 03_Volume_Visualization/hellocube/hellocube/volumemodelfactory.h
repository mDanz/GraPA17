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
};

