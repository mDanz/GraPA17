#pragma once


class TerrainModel;

class TerrainModelFactory
{
public:
	static TerrainModel* createFromFile(const QString& fileName);
};
