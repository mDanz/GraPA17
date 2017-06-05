
#include "viewportwidget.h"
#include "openglwidget.h"
#include "viewportmodel.h"

ViewPortWidget::ViewPortWidget(QWidget *parent) 
	: QWidget(parent)
	, m_model(nullptr)
{
	m_layout = new QVBoxLayout(this);

	initializeOpenGLWidgets();
	initializeLayout();

	// start with the single view
	selectPerspective();
	m_quadView->show();
	syncTop();
	m_currentLayout = QuadView;
	switchToView(SingleView);
}

ViewPortWidget::~ViewPortWidget()
{
	delete m_perspectiveWidget;
	delete m_frontWidget;
	delete m_leftWidget;
	delete m_topWidget;
	delete m_topView;
	delete m_bottomView;
	delete m_quadView;
	delete m_layout;
}

void ViewPortWidget::setModel(ViewPortModel* model)
{
	m_model = model;
	m_perspectiveWidget->setModel(m_model->getScene(), m_model->getCamera(0));
	m_frontWidget->setModel(m_model->getScene(), m_model->getCamera(1));
	m_leftWidget->setModel(m_model->getScene(), m_model->getCamera(2));
	m_topWidget->setModel(m_model->getScene(), m_model->getCamera(3));
}

OpenGLWidget* ViewPortWidget::getCurrentWidget() const
{
	return m_currentWidget;
}

CameraModel* ViewPortWidget::getCurrentCamera() const
{
	return m_model->getCurrentCamera();
}

void ViewPortWidget::update() const
{
	m_perspectiveWidget->update();
	if (m_currentLayout != SingleView)
	{
		m_frontWidget->update();
	}
	if (m_currentLayout == QuadView) 
	{
		m_topWidget->update();
		m_leftWidget->update();
	}
}

void ViewPortWidget::singleViewActivated()
{
	switchToView(SingleView);
}

void ViewPortWidget::dualViewActivated()
{
	switchToView(DualView);
}

void ViewPortWidget::quadViewActivated()
{
	switchToView(QuadView);
}

void ViewPortWidget::selectPerspective()
{
	select(m_perspectiveWidget);
}

void ViewPortWidget::selectFront()
{
	select(m_frontWidget);
}

void ViewPortWidget::selectLeft()
{
	select(m_leftWidget);
}

void ViewPortWidget::selectTop()
{
	select(m_topWidget);
}

void ViewPortWidget::initializeOpenGLWidgets()
{
	m_perspectiveWidget = new OpenGLWidget(this);
	m_frontWidget = new OpenGLWidget(this);
	m_leftWidget = new OpenGLWidget(this);
	m_topWidget = new OpenGLWidget(this);

	connect(m_perspectiveWidget, SIGNAL(clickedInside()), this, SLOT(selectPerspective()));
	connect(m_frontWidget, SIGNAL(clickedInside()), this, SLOT(selectFront()));
	connect(m_leftWidget, SIGNAL(clickedInside()), this, SLOT(selectLeft()));
	connect(m_topWidget, SIGNAL(clickedInside()), this, SLOT(selectTop()));
}

void ViewPortWidget::initializeLayout()
{
	m_quadView = new QSplitter(Qt::Vertical, this);
	m_topView = new QSplitter(Qt::Horizontal);
	m_topView->addWidget(m_perspectiveWidget);
	m_topView->addWidget(m_frontWidget);
	m_quadView->addWidget(m_topView);
	m_bottomView = new QSplitter(Qt::Horizontal);
	m_bottomView->addWidget(m_leftWidget);
	m_bottomView->addWidget(m_topWidget);
	m_quadView->addWidget(m_bottomView);

	m_bottomView->setChildrenCollapsible(false);
	m_topView->setChildrenCollapsible(false);
	m_quadView->setChildrenCollapsible(false);

	connect(m_bottomView, SIGNAL(splitterMoved(int, int)), this, SLOT(syncTop()));
	connect(m_topView, SIGNAL(splitterMoved(int, int)), this, SLOT(syncBottom()));

	m_layout->addWidget(m_quadView);
	this->setLayout(m_layout);
}

void ViewPortWidget::switchToView(ViewPortLayout type)
{
	hideWidgets(m_currentLayout);
	m_currentLayout = type;
	showWidgets(m_currentLayout);

	if (m_currentWidget->isHidden()) 
	{
		selectPerspective();
	}
}

void ViewPortWidget::hideWidgets(ViewPortWidget::ViewPortLayout type) const
{
	switch (type)
	{
	case DualView:
		m_frontWidget->hide();
		break;
	case QuadView:
		m_frontWidget->hide();
		m_bottomView->hide();
		break;
	case SingleView: break;
	default: break;
	}
}

void ViewPortWidget::showWidgets(ViewPortWidget::ViewPortLayout currentType) const
{
	switch (currentType)
	{
	case SingleView: break;
	case DualView:
		m_frontWidget->show();
		break;
	case QuadView:
		m_frontWidget->show();
		m_bottomView->show();
		break;
	default: break;
	}
}

void ViewPortWidget::select(OpenGLWidget* widget)
{
	m_perspectiveWidget->deselect();
	m_frontWidget->deselect();
	m_topWidget->deselect();
	m_leftWidget->deselect();

	m_currentWidget = widget;
	m_currentWidget->select();
	if (m_model) 
	{
		m_model->setCurrentCamera(m_currentWidget->getCamera());
	}

	update();
}

void ViewPortWidget::syncBottom() const
{
	m_bottomView->setSizes(m_topView->sizes());
}

void ViewPortWidget::syncTop() const
{
	m_topView->setSizes(m_bottomView->sizes());
}
