#include "mainwindow.h"

#include "openglwidget.h"

#include <QMessageBox>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QDockWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include "scenemodel.h"
#include "sceneitem.h"
#include <QUuid>
#include <QFileDialog>
#include "volumemodelfactory.h"
#include "viewportwidget.h"
#include "viewportmodel.h"
#include "sceneitemmodel.h"
#include "scenecontroller.h"

MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
    m_ui.setupUi(this);

	setWindowTitle("Hello Cube");

	initializeModel();
	initializeViewportWidget();
	initializeActions();
	initializeMenuBar();
	initializeToolBar();
	initializeStatusBar();
	initializeDockWidgets();
	initializeController();
	initializeActionConnections();
}

MainWindow::~MainWindow()
{
	delete m_scene;
	delete m_viewPortWidget;
	delete m_interactionModeActionGroup;
	delete m_viewModeActionGroup;
	delete m_exitAction;
	delete m_aboutAction;
	delete m_resetCameraAction;
	delete m_cameraModeAction;
	delete m_objManipulationModeAction;
	delete m_singleViewAction;
	delete m_dualViewAction;
	delete m_quadViewAction;
	delete m_addVolumeAction;
}

void MainWindow::showAboutBox() const
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("About Hello Cube!");
	msgBox.setText("Written by Marcel Danz.");
	msgBox.exec();
}

void MainWindow::initializeModel()
{
	m_scene = new SceneModel();
}

void MainWindow::initializeActions()
{
	m_exitAction = new QAction("E&xit", this);
	m_exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	m_aboutAction = new QAction("&About", this);

	m_resetCameraAction = new QAction("&Reset Camera", this);
	m_resetCameraAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	m_resetCameraAction->setIcon(QIcon(":/Resources/img/cam_home.png"));

	initializeInteractionModeActionGroup();
	initializeViewModeActionGroup();
	initializeGeometryActions();
}

void MainWindow::initializeInteractionModeActionGroup()
{
	m_interactionModeActionGroup = new QActionGroup(this);

	m_cameraModeAction = new QAction("&Camera", this);
	m_objManipulationModeAction = new QAction("&Object Manipulation", this);

	m_cameraModeAction->setIcon(QIcon(":/Resources/img/camera.png"));
	m_objManipulationModeAction->setIcon(QIcon(":/Resources/img/select.png"));

	m_cameraModeAction->setCheckable(true);
	m_objManipulationModeAction->setCheckable(true);

	m_interactionModeActionGroup->addAction(m_cameraModeAction);
	m_interactionModeActionGroup->addAction(m_objManipulationModeAction);
	m_cameraModeAction->setChecked(true);
}

void MainWindow::initializeViewModeActionGroup()
{
	m_viewModeActionGroup = new QActionGroup(this);

	m_singleViewAction = new QAction("&Single View", this);
	m_dualViewAction = new QAction("&Dual View", this);
	m_quadViewAction = new QAction("&Quad View", this);

	m_singleViewAction->setShortcut(QKeySequence(Qt::Key_1));
	m_dualViewAction->setShortcut(QKeySequence(Qt::Key_2));
	m_quadViewAction->setShortcut(QKeySequence(Qt::Key_4));

	m_singleViewAction->setIcon(QIcon(":/Resources/img/view-single.png"));
	m_dualViewAction->setIcon(QIcon(":/Resources/img/view-dual.png"));
	m_quadViewAction->setIcon(QIcon(":/Resources/img/viewports.png"));

	m_singleViewAction->setCheckable(true);
	m_dualViewAction->setCheckable(true);
	m_quadViewAction->setCheckable(true);

	m_viewModeActionGroup->addAction(m_singleViewAction);
	m_viewModeActionGroup->addAction(m_dualViewAction);
	m_viewModeActionGroup->addAction(m_quadViewAction);
	m_singleViewAction->setChecked(true);
}

void MainWindow::initializeGeometryActions()
{
	m_addSphereAction = new QAction("&Sphere", this);
	m_addCylinderAction = new QAction("&Cylinder", this);
	m_addConeAction = new QAction("&Cone", this);
	m_addTorusAction = new QAction("&Torus", this);
	m_addCubeAction = new QAction("&Cube", this);
	m_addVolumeAction = new QAction("&Volume", this);

	m_addSphereAction->setIcon(QIcon(":/Resources/img/sphere.png"));
	m_addCylinderAction->setIcon(QIcon(":/Resources/img/cylinder.png"));
	m_addConeAction->setIcon(QIcon(":/Resources/img/cone.png"));
	m_addTorusAction->setIcon(QIcon(":/Resources/img/torus.png"));
	m_addCubeAction->setIcon(QIcon(":/Resources/img/box.png"));
	m_addVolumeAction->setIcon(QIcon(":/Resources/img/sphere.png")); //todo correct icon for volume
}

void MainWindow::initializeController()
{
	m_sceneController = SceneController::getController();
	m_sceneController->setModelView(m_scene, m_viewPortWidget);
}

void MainWindow::initializeActionConnections() const
{
	connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
	//connect(m_resetCameraAction, SIGNAL(triggered()), m_currentGLWidget, SLOT(resetCamera()));//todo uncomment

	//todo fix connections push to controller
	connect(m_singleViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(singleViewActivated()));
	connect(m_dualViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(dualViewctivated()));
	connect(m_quadViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(quadViewActivated()));

	connect(m_cameraModeAction, SIGNAL(triggered()), m_sceneController, SLOT(cameraModeSelected()));
	connect(m_objManipulationModeAction, SIGNAL(triggered()), m_sceneController, SLOT(objectModeSelected()));
	connect(m_addCubeAction, SIGNAL(triggered()), m_sceneController, SLOT(cubeAdded()));
	connect(m_addSphereAction, SIGNAL(triggered()), m_sceneController, SLOT(sphereAdded()));
	connect(m_addCylinderAction, SIGNAL(triggered()), m_sceneController, SLOT(cylinderAdded()));
	connect(m_addConeAction, SIGNAL(triggered()), m_sceneController, SLOT(coneAdded()));
	connect(m_addTorusAction, SIGNAL(triggered()), m_sceneController, SLOT(torusAdded()));
	connect(m_addVolumeAction, SIGNAL(triggered()), m_sceneController, SLOT(volumeAdded()));
	connect(m_tessellationSlider, SIGNAL(valueChanged(int)), m_sceneController, SLOT(setTessellation(int)));
}

void MainWindow::initializeMenuBar()
{
	m_menuBar = new QMenuBar();

	initializeFileMenu();
	initializeInteractionModeMenu();
	initializeViewModeMenu();
	initializeGeometryMenu();
	initializeAboutMenu();

	setMenuBar(m_menuBar);
}

void MainWindow::initializeFileMenu()
{
	m_fileMenu = new QMenu("&File");
	m_fileMenu->addAction(m_exitAction);

	m_menuBar->addMenu(m_fileMenu);
}

void MainWindow::initializeInteractionModeMenu()
{
	m_interactionModeMenu = new QMenu("&Interaction");
	m_interactionModeMenu->addAction(m_cameraModeAction);
	m_interactionModeMenu->addAction(m_objManipulationModeAction);

	m_menuBar->addMenu(m_interactionModeMenu);
}

void MainWindow::initializeViewModeMenu()
{
	m_viewModeMenu = new QMenu("&View");
	m_viewModeMenu->addAction(m_singleViewAction);
	m_viewModeMenu->addAction(m_dualViewAction);
	m_viewModeMenu->addAction(m_quadViewAction);

	m_menuBar->addMenu(m_viewModeMenu);
}

void MainWindow::initializeGeometryMenu()
{
	m_geometryMenu = new QMenu("&Geometry");
	m_geometryMenu->addAction(m_addCubeAction);
	m_geometryMenu->addAction(m_addSphereAction);
	m_geometryMenu->addAction(m_addCylinderAction);
	m_geometryMenu->addAction(m_addConeAction);
	m_geometryMenu->addAction(m_addTorusAction);
	m_geometryMenu->addAction(m_addVolumeAction);

	m_menuBar->addMenu(m_geometryMenu);
}

void MainWindow::initializeAboutMenu()
{
	m_aboutMenu = new QMenu("&About");
	m_aboutMenu->addAction(m_aboutAction);

	m_menuBar->addMenu(m_aboutMenu);
}

void MainWindow::initializeToolBar()
{
	
	initializeGeneralToolBar();
	initializeGeometryToolBar();
}

void MainWindow::initializeGeneralToolBar()
{
	m_generalToolBar = new QToolBar("View Mode", this);
	m_generalToolBar->addAction(m_cameraModeAction);
	m_generalToolBar->addAction(m_objManipulationModeAction);

	auto viewModeButton = new QToolButton();
	viewModeButton->setMenu(m_viewModeMenu);
	viewModeButton->setPopupMode(QToolButton::InstantPopup);
	viewModeButton->setIcon(QIcon(":/Resources/img/viewports.png"));
	m_generalToolBar->addWidget(viewModeButton);

	m_generalToolBar->addAction(m_resetCameraAction);

	m_tessellationSlider = createSlider();
	m_generalToolBar->addWidget(m_tessellationSlider);

	addToolBar(m_generalToolBar);
}

void MainWindow::initializeGeometryToolBar()
{
	m_geometryToolBar = new QToolBar("Geometry", this);
	m_geometryToolBar->addAction(m_addCubeAction);
	m_geometryToolBar->addAction(m_addSphereAction);
	m_geometryToolBar->addAction(m_addCylinderAction);
	m_geometryToolBar->addAction(m_addConeAction);
	m_geometryToolBar->addAction(m_addTorusAction);
	m_geometryToolBar->addAction(m_addVolumeAction);

	addToolBar(m_geometryToolBar);
}

void MainWindow::initializeStatusBar()
{
	auto label = new QLabel("<none>");
    m_ui.statusBar->addWidget(label); //had to use m_ui otherwise statusBar would be at top of screen.
}

void MainWindow::initializeDockWidgets()
{
	initializeOutliner();
}

void MainWindow::initializeViewportWidget()
{ 
	m_viewPortWidget = new ViewPortWidget(this);
	m_viewPortWidget->setModel(new ViewPortModel(m_scene));
	setCentralWidget(m_viewPortWidget);
}

void MainWindow::initializeOutliner()
{
	m_outlinerDock = new QDockWidget("Outliner", this);

	m_outlinerTreeView = new QTreeView(m_outlinerDock);
	m_outlinerTreeView->setModel(new SceneItemModel(m_scene->getRoot()));
	
	m_outlinerDock->setWidget(m_outlinerTreeView);

	connect(m_outlinerTreeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(changeSelection(QModelIndex, QModelIndex)));
	addDockWidget(Qt::LeftDockWidgetArea, m_outlinerDock);
}

QSlider* MainWindow::createSlider()
{
	auto slider = new QSlider(Qt::Horizontal, this);
	slider->setRange(0, 6);
	slider->setSingleStep(1);
	slider->setTickPosition(QSlider::TicksRight);
	slider->setValue(0);
	//slider->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Ignored));
	return slider;
}

