
#include "terraineditordock.h"
#include <QVBoxLayout>
#include <qslider.h>
#include <QLabel>

TerrainEditorDock::TerrainEditorDock(QString title, QWidget* parent) 
	: QDockWidget(title, parent)
	, m_terrain(nullptr)
{
	m_layout = new QGridLayout(this);

	initializeSliders();
	initializeCheckBoxes();

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
	m_heightSlider->setSliderPosition(m_terrain->getHeightScale());
	m_widthSlider->setSliderPosition(m_terrain->getWidthScale());
}

void TerrainEditorDock::initializeSliders()
{
	m_heightSlider = createSlider();
	m_widthSlider = createSlider();

	auto heightLabel = new QLabel("Height:");
	auto widthLabel = new QLabel("Width:");

	m_layout->addWidget(heightLabel,0, 0);
	m_layout->addWidget(m_heightSlider,1, 0);
	m_layout->addWidget(widthLabel, 2, 0);
	m_layout->addWidget(m_widthSlider, 3, 0);

	connect(m_heightSlider, SIGNAL(valueChanged(int)), this, SLOT(setTerrainHeight(int)));
	connect(m_widthSlider, SIGNAL(valueChanged(int)), this, SLOT(setTerrainWidth(int)));
}

void TerrainEditorDock::initializeCheckBoxes()
{
	m_wireframeEnabledBox = new QCheckBox("Wireframe", this);
	m_layout->addWidget(m_wireframeEnabledBox);
	connect(m_wireframeEnabledBox, SIGNAL(clicked(bool)), this, SLOT(updateWireframeMode()));

}

QSlider* TerrainEditorDock::createSlider()
{
	auto slider = new QSlider(Qt::Horizontal, this);
	slider->setRange(1, 500);
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

void TerrainEditorDock::updateWireframeMode() const
{
	if (!m_terrain)
	{
		return;
	}
	m_terrain->updateWireframEnabled(m_wireframeEnabledBox->isChecked());
}
