#pragma once

#include <QActionGroup>
#include "ui_mainwindow.h"
#include "openglwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

	public slots:
	void showAboutBox();

private:
    Ui::MainWindowClass ui;
	OpenGLWidget *openGLWidget;

	QMenuBar *menuBar;

	QMenu *fileMenu;
	QMenu *interactionModeMenu;
	QMenu *viewModeMenu;
	QMenu *aboutMenu;

	QToolBar *toolBar;

	QStatusBar *statusBar;

	QDockWidget *outlinerDock;

	QActionGroup *interactionModeActionGroup;
	QActionGroup *viewModeActionGroup;

	QAction *exitAction;
	QAction *aboutAction;
	QAction *resetCameraAction;
	QAction *cameraModeAction;
	QAction *objManipulationModeAction;
	QAction *singleViewAction;
	QAction *dualViewAction;
	QAction *quadViewAction;

	QSlider *tesselationSlider;

	QTreeView *outlinerTreeView;

	void initializeActions();
	void initializeInteractionModeActionGroup();
	void initializeViewModeActionGroup();
	void initializeActionConnections();
	void initializeMenuBar();
	void initializeFileMenu();
	void initializeInteractionModeMenu();
	void initializeViewModeMenu();
	void initializeAboutMenu();
	void initializeToolBar();
	void initializeStatusBar();
	void initializeDockWidgets();
	void initializeOutliner();
	QSlider* createSlider();
};
