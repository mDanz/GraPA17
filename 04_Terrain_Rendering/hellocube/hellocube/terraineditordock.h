#pragma once
#include <QDockWidget>
#include "terrainmodel.h"
#include <QCheckBox>
#include <QVBoxLayout>


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
	void initializeCheckBoxes();
	QSlider* createSlider(int start = 1, int end = 500);

	QGridLayout *m_layout;

	QSlider *m_heightSlider;
	QSlider *m_widthSlider;
	QSlider *m_fallOffSlider;

	QCheckBox *m_wireframeEnabledBox;

	TerrainModel *m_terrain;

private slots:
	void setTerrainHeight(int height) const;
	void setTerrainWidth(int width) const;
	void setTerrainFallOff(int falloff) const;
	void updateWireframeMode() const;

};
