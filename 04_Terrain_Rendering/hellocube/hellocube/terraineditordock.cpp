
#include "terraineditordock.h"
#include <QVBoxLayout>
#include <qslider.h>

TerrainEditorDock::TerrainEditorDock(QString title, QWidget* parent) : QDockWidget(title, parent)
{
	m_layout = new QGridLayout(this);

	initializeSliders();

	auto widget = new QWidget(this);
	widget->setLayout(m_layout);
	setWidget(widget);
}

TerrainEditorDock::~TerrainEditorDock()
{
	m_terrain = nullptr;
}

void TerrainEditorDock::updateTerrain(TerrainModel* terrain)
{
	m_terrain = terrain;
	m_heightSlider->setTickPosition(m_terrain->getHeightScale());
	m_widthSlider->setTickPosition(m_terrain->getWidthScale());
}

void TerrainEditorDock::initializeSliders()
{
	m_heightSlider = createSlider();
	m_widthSlider = createSlider();

	m_layout->addWidget(m_heightSlider);
	m_layout->addWidget(m_widthSlider);

	connect(m_heightSlider, SIGNAL(valueChanged(int)), this, SLOT(setTerrainHeight(int)));
	connect(m_widthSlider, SIGNAL(valueChanged(int)), this, SLOT(setTerrainWidth(int)));
}

QSlider* TerrainEditorDock::createSlider()
{
	auto slider = new QSlider(Qt::Horizontal, this);
	slider->setRange(1, 100);
	slider->setSingleStep(1);
	slider->setTickPosition(QSlider::TicksRight);
	slider->setValue(1);
	//slider->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Ignored));
	return slider;
}

void TerrainEditorDock::setTerrainHeight(int height) const
{
	if (!m_terrain)
	{
		return;
	}
	m_terrain->setHeightScale(height);
}

void TerrainEditorDock::setTerrainWidth(int width) const
{
	if (!m_terrain)
	{
		return;
	}
	m_terrain->setWidthScale(width);
}
