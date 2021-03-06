#pragma once

#include <QActionGroup>
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include <QSplitter>
#include <QStackedWidget>
#include "scenemodel.h"
#include "modellingtoolmodel.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

public slots:
	void showAboutBox();
	void singleViewModeActivated();
	void dualViewModeActivated();
	void quadViewModeActivated();
	void cubeAdded();
	void sphereAdded();
	void cylinderAdded();
	void coneAdded();
	void torusAdded();
	void selectedObjectChanged(const QModelIndex &current, const QModelIndex &previous) const;

private:
	ModellingToolModel *m_model;

    Ui::MainWindowClass m_ui;
	QStackedWidget *m_stackedWidget;
	QSplitter *m_dualViewSplitter;
	QSplitter *m_quadViewSplitter;
	QSplitter *m_topRowSplitter;
	QSplitter *m_bottomRowSplitter;
	OpenGLWidget *m_currentGLWidget;
	OpenGLWidget *m_singlePerspectiveView;
	OpenGLWidget *m_perspectiveGLWidgetDual;
	OpenGLWidget *m_perspectiveGLWidgetQuad;
	OpenGLWidget *m_frontGLWidgetDual;
	OpenGLWidget *m_frontGLWidgetQuad;
	OpenGLWidget *m_leftGLWidgetQuad;
	OpenGLWidget *m_topGLWidgetQuad;

	QMenuBar *m_menuBar;

	QMenu *m_fileMenu;
	QMenu *m_interactionModeMenu;
	QMenu *m_viewModeMenu;
	QMenu *m_geometryMenu;
	QMenu *m_aboutMenu;

	QToolBar *m_generalToolBar;
	QToolBar *m_geometryToolBar;

	QStatusBar *m_statusBar;

	QDockWidget *m_outlinerDock;

	QActionGroup *m_interactionModeActionGroup;
	QActionGroup *m_viewModeActionGroup;

	QAction *m_exitAction;
	QAction *m_aboutAction;
	QAction *m_resetCameraAction;
	QAction *m_cameraModeAction;
	QAction *m_objManipulationModeAction;
	QAction *m_singleViewAction;
	QAction *m_dualViewAction;
	QAction *m_quadViewAction;
	QAction *m_addSphereAction;
	QAction *m_addCylinderAction;
	QAction *m_addConeAction;
	QAction *m_addTorusAction;
	QAction *m_addCubeAction;

	QSlider *m_tesselationSlider;

	QTreeView *m_outlinerTreeView;

	void initializeModel();
	void initializeActions();
	void initializeInteractionModeActionGroup();
	void initializeViewModeActionGroup();
	void initializeGeometryActions();
	void initializeActionConnections();
	void initializeMenuBar();
	void initializeFileMenu();
	void initializeInteractionModeMenu();
	void initializeViewModeMenu();
	void initializeGeometryMenu();
	void initializeAboutMenu();
	void initializeToolBar();
	void initializeGeneralToolBar();
	void initializeGeometryToolBar();
	void initializeStatusBar();
	void initializeDockWidgets();
	void initializeViewportLayouts();
	void initializeOutliner();
	QSlider* createSlider();
};
