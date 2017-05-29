
#include "transferfunctioneditordock.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <qtextstream.h>
#include "transferfunction.h"

TransferFunctionEditorDock::TransferFunctionEditorDock(QWidget* parent, TransferFunction* transferFunc)
	: QDockWidget(parent)
{
	m_layout = new QGridLayout(this);

	initializeCanvas();
	initializeCheckBoxes();
	initializeButtons();

	QWidget *widget = new QWidget(this);
	widget->setLayout(m_layout);
	this->setWidget(widget);
	m_transferFunc = transferFunc;
	//this->volume = v; //todo get currently selected volume

}

TransferFunctionEditorDock::~TransferFunctionEditorDock()
{
	delete m_transferFunc;
	delete m_canvas;
}

void TransferFunctionEditorDock::initializeCanvas()
{
	m_canvas = new TransferFunctionCanvas(this);
	m_layout->addWidget(m_canvas, 0, 0, 1, 4);
	m_canvas->show();
	m_canvas->renderTransferFunction(m_transferFunc, m_volume);
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

	connect(m_redBox, SIGNAL(clicked(bool)), this, SLOT(updateBoxes()));
	connect(m_greenBox, SIGNAL(clicked(bool)), this, SLOT(updateBoxes()));
	connect(m_blueBox, SIGNAL(clicked(bool)), this, SLOT(updateBoxes()));
	connect(m_alphaBox, SIGNAL(clicked(bool)), this, SLOT(updateBoxes()));
}

void TransferFunctionEditorDock::initializeButtons()
{
	m_smoothButton = new QPushButton(QString("Smooth"), this);
	m_resetButton = new QPushButton(QString("Reset"), this);
	
	m_layout->addWidget(m_smoothButton, 2, 0, 1, 2);
	m_layout->addWidget(m_resetButton, 2, 2, 1, 2);

	connect(m_smoothButton, SIGNAL(clicked(bool)), this, SLOT(smoothTransferFunction()));
	connect(m_resetButton, SIGNAL(clicked(bool)), this, SLOT(defaultTransferFunction()));
}
