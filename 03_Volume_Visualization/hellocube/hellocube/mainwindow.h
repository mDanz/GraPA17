#pragma once

#include <QActionGroup>
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include "scenemodel.h"

class ViewPortWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

public slots:
	void showAboutBox();
	void volumeAdded();

	void selectedObjectChanged(const QModelIndex &current, const QModelIndex &previous) const;

private:
	SceneModel *m_model;

    Ui::MainWindowClass m_ui;
	ViewPortWidget *m_viewPortWidget;

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
	QAction *m_addVolumeAction;

	//QSlider *m_tesselationSlider;
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
	void initializeViewportWidget();
	void initializeOutliner();
	//QSlider* createSlider();
};
