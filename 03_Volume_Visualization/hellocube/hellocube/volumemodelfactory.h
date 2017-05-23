#pragma once
#include <memory>
#include <vector>


class QString;
class VolumeModel;

class VolumeModelFactory
{
public:
	static VolumeModel* createFromFile(const QString& fileName);

private:
	static std::vector<unsigned char> readFile(const QString& fileName);
};

