#pragma once

#include <QActionGroup>
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include <QGridLayout>
#include <QSplitter>
#include <QStackedWidget>

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

private:
    Ui::MainWindowClass m_ui;
	QStackedWidget *m_stackedWidget;
	QSplitter *m_dualViewSplitter;
	QSplitter *m_quadViewSplitter;
	QSplitter *m_topRowSplitter;
	QSplitter *m_bottomRowSplitter;
	OpenGLWidget *m_openGLWidget; //todo change to selected glwidget
	OpenGLWidget *m_singlePerspreciveView;
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
	QMenu *m_aboutMenu;

	QToolBar *m_toolBar;

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

	QSlider *m_tesselationSlider;

	QTreeView *m_outlinerTreeView;

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
	void initializeViewportLayouts();
	void initializeOutliner();
	QSlider* createSlider();
};
