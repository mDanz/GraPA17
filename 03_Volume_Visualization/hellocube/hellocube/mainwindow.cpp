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
#include <QSplitter>
#include <QStackedWidget>
#include "sceneitem.h"
#include "objectid.h"
#include <QUuid>
#include <QFileDialog>
#include "volumemodelfactory.h"
#include "viewportwidget.h"
#include "viewportmodel.h"
#include "sceneitemmodel.h"

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
	initializeActionConnections();
}

MainWindow::~MainWindow()
{
	delete m_model;
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

void MainWindow::volumeAdded()
{
	auto fileName = QFileDialog::getOpenFileName(this, tr("Load Volume Data"), "./", tr("Raw Files (*.raw)"));
	//m_model->setVolume(VolumeModelFactory::createFromFile(fileName)); //todo fix volume to tree adding
}

void MainWindow::selectedItemChanged(const QModelIndex& current, const QModelIndex& previous) const
{
	m_model->selectItem(static_cast<SceneItem*>(current.internalPointer()));
	// auto name = m_model->getScene()->getSelectedItem()->getName();//todo fix name display
	// m_ui.statusBar->messageChanged(name);
}

void MainWindow::initializeModel()
{
	m_model = new SceneModel();
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
	m_addVolumeAction = new QAction("Add &Volume", this);
	m_addVolumeAction->setIcon(QIcon(":/Resources/img/sphere.png"));
}

void MainWindow::initializeActionConnections()
{
	connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
	//connect(m_resetCameraAction, SIGNAL(triggered()), m_currentGLWidget, SLOT(resetCamera()));//todo uncomment

	//todo fix connections push to controller
	//connect(m_cameraModeAction, SIGNAL(triggered()), m_currentGLWidget, SLOT(selectedCameraMode()));
	//connect(m_objManipulationModeAction, SIGNAL(triggered()), m_currentGLWidget, SLOT(selectedObjManipulationMode()));
	connect(m_singleViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(singleViewActivated()));
	connect(m_dualViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(dualViewctivated()));
	connect(m_quadViewAction, SIGNAL(triggered()), m_viewPortWidget, SLOT(quadViewActivated()));

	connect(m_addVolumeAction, SIGNAL(triggered()), this, SLOT(volumeAdded()));
}

void MainWindow::initializeMenuBar()
{
	m_menuBar = new QMenuBar();

	initializeFileMenu();
	initializeInteractionModeMenu();
	initializeViewModeMenu();
	//initializeGeometryMenu();
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

	//m_tesselationSlider = createSlider();
	//m_generalToolBar->addWidget(m_tesselationSlider);

	addToolBar(m_generalToolBar);
}

void MainWindow::initializeGeometryToolBar()
{
	m_geometryToolBar = new QToolBar("Geometry", this);
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
	m_viewPortWidget->setModel(new ViewPortModel(m_model));
	setCentralWidget(m_viewPortWidget);
}

void MainWindow::initializeOutliner()
{
	m_outlinerDock = new QDockWidget("Outliner", this);

	m_outlinerTreeView = new QTreeView(m_outlinerDock);
	m_outlinerTreeView->setModel(new SceneItemModel(m_model->getRoot()));
	
	m_outlinerDock->setWidget(m_outlinerTreeView);

	connect(m_outlinerTreeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(changeSelection(QModelIndex, QModelIndex)));
	addDockWidget(Qt::LeftDockWidgetArea, m_outlinerDock);
}

//QSlider* MainWindow::createSlider()
//{
//	auto slider = new QSlider(Qt::Horizontal);
//	slider->setRange(0, 6);
//	slider->setSingleStep(1);
//	slider->setTickPosition(QSlider::TicksRight);
//	slider->setValue(0);
//	//slider->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Ignored));
//	return slider;
//}

