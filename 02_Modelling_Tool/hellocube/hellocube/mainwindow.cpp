#include "mainwindow.h"

#include "openglwidget.h"

#include <QMessageBox>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>

MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);

	setWindowTitle("Hello Cube");
    openGLWidget = new OpenGLWidget(this);

	initializeActions();
	initializeMenuBar();
	initializeToolBar();	//todo seperate toolbars
	initializeStatusBar();

	setCentralWidget(openGLWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::showAboutBox()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("About Hello Cube!");
	msgBox.setText("Written by Marcel Danz.");
	msgBox.exec();
}

void MainWindow::initializeActions()
{
	exitAction = new QAction("E&xit", this);
	exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	aboutAction = new QAction("&About", aboutMenu);

	resetCameraAction = new QAction("&Reset Camera", toolBar);
	resetCameraAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	resetCameraAction->setIcon(QIcon(":/Resources/img/cam_home.png"));

	initializeInteractionModeActionGroup();
	initializeViewModeActionGroup();
	initializeActionConnections();
}

void MainWindow::initializeInteractionModeActionGroup()
{
	interactionModeActionGroup = new QActionGroup(this);

	cameraModeAction = new QAction("&Camera", interactionModeMenu);
	objManipulationModeAction = new QAction("&Object Manipulation", interactionModeMenu);

	cameraModeAction->setIcon(QIcon(":/Resources/img/camera.png"));
	objManipulationModeAction->setIcon(QIcon(":/Resources/img/select.png"));

	cameraModeAction->setCheckable(true);
	objManipulationModeAction->setCheckable(true);

	interactionModeActionGroup->addAction(cameraModeAction);
	interactionModeActionGroup->addAction(objManipulationModeAction);
	cameraModeAction->setChecked(true);
}

void MainWindow::initializeViewModeActionGroup()
{
	viewModeActionGroup = new QActionGroup(this);

	singleViewAction = new QAction("&Single View", viewModeMenu);
	dualViewAction = new QAction("&Dual View", viewModeMenu);
	quadViewAction = new QAction("&Quad View", viewModeMenu);

	singleViewAction->setShortcut(QKeySequence(Qt::Key_1));
	dualViewAction->setShortcut(QKeySequence(Qt::Key_2));
	quadViewAction->setShortcut(QKeySequence(Qt::Key_4));

	singleViewAction->setIcon(QIcon(":/Resources/img/view-single.png"));
	dualViewAction->setIcon(QIcon(":/Resources/img/view-dual.png"));
	quadViewAction->setIcon(QIcon(":/Resources/img/viewports.png"));

	singleViewAction->setCheckable(true);
	dualViewAction->setCheckable(true);
	quadViewAction->setCheckable(true);

	viewModeActionGroup->addAction(singleViewAction);
	viewModeActionGroup->addAction(dualViewAction);
	viewModeActionGroup->addAction(quadViewAction);
	singleViewAction->setChecked(true);
}

void MainWindow::initializeActionConnections()
{
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
	connect(resetCameraAction, SIGNAL(triggered()), openGLWidget, SLOT(resetCamera()));
	//todo connect signals
	//connect(cameraModeAction, SIGNAL(triggered()), target, SLOT(cameraModeActivated()));
	//connect(objManipulationModeAction, SIGNAL(triggered()), target, SLOT(objManipulationModeActivated()));
	//connect(singleViewAction, SIGNAL(triggered()), target, SLOT(singleViewModeActivated()));
	//connect(dualViewAction, SIGNAL(triggered()), target, SLOT(dualViewModeActivated()));
	//connect(quadViewAction, SIGNAL(triggered()), target, SLOT(QuadViewModeActivated()));
}

void MainWindow::initializeMenuBar()
{
	menuBar = new QMenuBar();

	initializeFileMenu();
	initializeInteractionModeMenu();
	initializeViewModeMenu();
	initializeAboutMenu();

	setMenuBar(menuBar);
}

void MainWindow::initializeFileMenu()
{
	fileMenu = new QMenu("&File");
	fileMenu->addAction(exitAction);

	menuBar->addMenu(fileMenu);
}

void MainWindow::initializeInteractionModeMenu()
{
	interactionModeMenu = new QMenu("&Interaction");
	interactionModeMenu->addAction(cameraModeAction);
	interactionModeMenu->addAction(objManipulationModeAction);

	menuBar->addMenu(interactionModeMenu);
}

void MainWindow::initializeViewModeMenu()
{
	viewModeMenu = new QMenu("&View");
	viewModeMenu->addAction(singleViewAction);
	viewModeMenu->addAction(dualViewAction);
	viewModeMenu->addAction(quadViewAction);

	menuBar->addMenu(viewModeMenu);
}

void MainWindow::initializeAboutMenu()
{
	aboutMenu = new QMenu("&About");
	aboutMenu->addAction(aboutAction);

	menuBar->addMenu(aboutMenu);
}

void MainWindow::initializeToolBar()
{
	toolBar = new QToolBar("View Mode");
	toolBar->addAction(cameraModeAction);
	toolBar->addAction(objManipulationModeAction);


	auto viewModeButton = new QToolButton();
	viewModeButton->setMenu(viewModeMenu);
	viewModeButton->setPopupMode(QToolButton::InstantPopup);
	viewModeButton->setIcon(QIcon(":/Resources/img/viewports.png"));
	toolBar->addWidget(viewModeButton);

	tesselationSlider = createSlider();
	toolBar->addWidget(tesselationSlider);

	toolBar->addAction(resetCameraAction);

	addToolBar(toolBar);

    connect(tesselationSlider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(setTesselation(int)));
}

void MainWindow::initializeStatusBar()
{
	auto label = new QLabel();
    ui.statusBar->addWidget(label); //had to use ui otherwise statusBar would be at top of screen.
}

QSlider* MainWindow::createSlider()
{
	auto slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 6);
	slider->setSingleStep(1);
	slider->setTickPosition(QSlider::TicksRight);
	slider->setValue(0);
	//slider->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Ignored));
	return slider;
}

