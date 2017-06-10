#pragma once
#include <QDockWidget>
#include "terrainmodel.h"


class QSlider;

class TerrainEditorDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit TerrainEditorDock(QString title = "", QWidget *parent = nullptr);
	~TerrainEditorDock();

public slots:
	void updateTerrain(TerrainModel *terrain);

private:
	void initializeSliders();
	QSlider* createSlider();

	QLayout *m_layout;

	QSlider *m_heightSlider;
	QSlider *m_widthSlider;

	TerrainModel *m_terrain;

private slots:
	void setTerrainHeight(int height) const;
	void setTerrainWidth(int width) const;

};
