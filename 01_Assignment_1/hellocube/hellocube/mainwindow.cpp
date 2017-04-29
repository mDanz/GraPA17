#include "mainwindow.h"

#include "openglwidget.h"

#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle("Hello Cube");

	initializeMenuBar();
	initializeToolBar();
	initializeStatusBar();

	setCentralWidget(new OpenGLWidget(this));
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

void MainWindow::initializeMenuBar()
{
	menuBar = new QMenuBar();

	initializeFileMenu();
	initializeShadingMenu();
	initializeAboutMenu();

	setMenuBar(menuBar);
}

void MainWindow::initializeFileMenu()
{
	fileMenu = new QMenu("&File");

	exitAction = new QAction("E&xit", fileMenu);
	exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	fileMenu->addAction(exitAction);

	menuBar->addMenu(fileMenu);

	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::initializeShadingMenu()
{
	shadingMenu = new QMenu("&Shading");
	shadingActionGroup = new QActionGroup(this);

	initializeShadingActionGroup();

	shadingMenu->addAction(noneShadingAction);
	shadingMenu->addAction(flatShadingAction);
	shadingMenu->addAction(gouraudShadingAction);
	shadingMenu->addAction(phongShadingAction);

	menuBar->addMenu(shadingMenu);

	connect(noneShadingAction, SIGNAL(triggered()), this, SLOT(OpenGLWidget::wireframeShading()));
	connect(flatShadingAction, SIGNAL(triggered()), this, SLOT(OpenGLWidget::flatShading()));
	connect(gouraudShadingAction, SIGNAL(triggered()), this, SLOT(OpenGLWidget::gouraudShading()));
	connect(phongShadingAction, SIGNAL(triggered()), this, SLOT(OpenGLWidget::phongShading()));
}

void MainWindow::initializeShadingActionGroup()
{
	noneShadingAction = new QAction("&None (Wireframe)", shadingMenu);
	flatShadingAction = new QAction("&Flat", shadingMenu);
	gouraudShadingAction = new QAction("&Gouraud)", shadingMenu);
	phongShadingAction = new QAction("&Phong)", shadingMenu);

	noneShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
	flatShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	gouraudShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
	phongShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));

	noneShadingAction->setCheckable(true);
	flatShadingAction->setCheckable(true);
	gouraudShadingAction->setCheckable(true);
	phongShadingAction->setCheckable(true);

	noneShadingAction->setIcon(QIcon(":/img/wireframe.png"));
	flatShadingAction->setIcon(QIcon(":/img/flat.png"));
	gouraudShadingAction->setIcon(QIcon(":/img/gouraud.png"));
	phongShadingAction->setIcon(QIcon(":/img/phong.png"));

	shadingActionGroup->addAction(noneShadingAction);
	shadingActionGroup->addAction(flatShadingAction);
	shadingActionGroup->addAction(gouraudShadingAction);
	shadingActionGroup->addAction(phongShadingAction);
	flatShadingAction->setChecked(true);
}

void MainWindow::initializeAboutMenu()
{
	aboutMenu = new QMenu("&About");
	aboutAction = new QAction("&About", aboutMenu);
	aboutMenu->addAction(aboutAction);
	menuBar->addMenu(aboutMenu);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
}

void MainWindow::initializeToolBar()
{
	shadingToolBar = new QToolBar("Shading");
	shadingToolBar->addAction(noneShadingAction);
	shadingToolBar->addAction(flatShadingAction);
	shadingToolBar->addAction(gouraudShadingAction);
	shadingToolBar->addAction(phongShadingAction);

	tesselationSlider = new QSlider(this);
	shadingToolBar->addWidget(tesselationSlider);

	resetCameraAction = new QAction("&Reset Camera", shadingToolBar);
	resetCameraAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	resetCameraAction->setIcon(QIcon(":/img/cam_home.png"));
	shadingToolBar->addAction(resetCameraAction);

	addToolBar(shadingToolBar);

	connect(tesselationSlider, SIGNAL(valueChanged()), this, SLOT(OpenGLWidget::setTesselation()));
	connect(resetCameraAction, SIGNAL(triggered()), this, SLOT(OpenGLWidget::resetCamera()));
}

void MainWindow::initializeStatusBar()
{
	auto label = new QLabel("Ready");
	ui.statusBar->addWidget(label); //had to use ui 
}

