
#include "transferfunctioneditordock.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>

TransferFunctionEditorDock::TransferFunctionEditorDock(QString title, QWidget* parent)
	: QDockWidget(title, parent)
	, m_volume(nullptr)
{
	m_layout = new QGridLayout(this);

	initializeCanvas();
	initializeCheckBoxes();
	initializeButtons();

	//setLayout(m_layout);
	auto widget = new QWidget(this);
	widget->setLayout(m_layout);
	setWidget(widget);
}

TransferFunctionEditorDock::~TransferFunctionEditorDock()
{
	delete m_volume;
	delete m_canvas;
}

void TransferFunctionEditorDock::updateVolume(VolumeModel *volume)
{
	m_volume = volume;
	connect(m_volume->getTransferFunction(), SIGNAL(transferFunctionChanged()), this, SLOT(updateCanvas()));
	updateCanvas();
}

void TransferFunctionEditorDock::initializeCanvas()
{
	m_canvas = new TransferFunctionCanvas(this);
	m_layout->addWidget(m_canvas, 0, 0, 1, 4);
	m_canvas->show();
	updateCanvas();
}

void TransferFunctionEditorDock::initializeCheckBoxes()
{
	m_redBox = new QCheckBox("Red", this);
	m_greenBox = new QCheckBox("Green", this);
	m_blueBox = new QCheckBox("Blue", this);
	m_alphaBox = new QCheckBox("Alpha", this);

	m_layout->addWidget(m_redBox, 1, 0);
	m_layout->addWidget(m_greenBox, 1, 1);
	m_layout->addWidget(m_blueBox, 1, 2);
	m_layout->addWidget(m_alphaBox, 1, 3);

	connect(m_redBox, SIGNAL(clicked(bool)), this, SLOT(updateColorChannelBoxes()));
	connect(m_greenBox, SIGNAL(clicked(bool)), this, SLOT(updateColorChannelBoxes()));
	connect(m_blueBox, SIGNAL(clicked(bool)), this, SLOT(updateColorChannelBoxes()));
	connect(m_alphaBox, SIGNAL(clicked(bool)), this, SLOT(updateColorChannelBoxes()));
}

void TransferFunctionEditorDock::initializeButtons()
{
	m_smoothButton = new QPushButton(QString("Smooth"), this);
	m_resetButton = new QPushButton(QString("Reset"), this);
	
	m_layout->addWidget(m_smoothButton, 2, 0, 1, 2);
	m_layout->addWidget(m_resetButton, 2, 2, 1, 2);

	connect(m_smoothButton, SIGNAL(clicked(bool)), this, SLOT(smoothTransferFunction()));
	connect(m_resetButton, SIGNAL(clicked(bool)), this, SLOT(resetTransferFunction()));
}

void TransferFunctionEditorDock::updateColorChannelBoxes() const
{
	m_canvas->setAffectedColorChannels(m_redBox->isChecked(), m_greenBox->isChecked(), m_blueBox->isChecked(), m_alphaBox->isChecked());
}

void TransferFunctionEditorDock::updateCanvas() const
{
	m_canvas->reRenderTransferFunction(m_volume);
}

void TransferFunctionEditorDock::smoothTransferFunction() const
{
	m_volume->getTransferFunction()->smoothSelectedColorChannels(m_redBox->isChecked(), m_greenBox->isChecked(), m_blueBox->isChecked(), m_alphaBox->isChecked());
}

void TransferFunctionEditorDock::resetTransferFunction() const
{
	m_volume->getTransferFunction()->resetSelectedColorChannels(m_redBox->isChecked(), m_greenBox->isChecked(), m_blueBox->isChecked(), m_alphaBox->isChecked());
}
