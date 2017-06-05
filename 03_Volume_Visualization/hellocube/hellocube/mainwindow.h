#pragma once

#include <QActionGroup>
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include "scenemodel.h"
#include "sceneitemmodel.h"
#include "transferfunctioneditordock.h"

class SceneController;
class ViewPortWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

public slots:
	static void showAboutBox();

private:
	void initializeModel();
	void initializeActions();
	void initializeInteractionModeActionGroup();
	void initializeViewModeActionGroup();
	void initializeGeometryActions();
	void initializeController();
	void initializeActionConnections() const;
	void initializeMenuBar();
	void initializeFileMenu();
	void initializeInteractionModeMenu();
	void initializeViewModeMenu();
	void initializeGeometryMenu();
	void initializeAboutMenu();
	void initializeToolBar();
	void initializeGeneralToolBar();
	void initializeOptionsToolBar();
	void initializeGeometryToolBar();
	void initializeStatusBar() const;
	void initializeDockWidgets();
	void initializeViewportWidget();
	void initializeOutliner();
	void initializeTransferFunctionEditor();
	QSlider* createSlider();

	SceneModel *m_scene;
	SceneItemModel *m_outlinerModel;

    Ui::MainWindowClass m_ui;
	ViewPortWidget *m_viewPortWidget;

	QMenuBar *m_menuBar;
	QMenu *m_fileMenu;
	QMenu *m_interactionModeMenu;
	QMenu *m_viewModeMenu;
	QMenu *m_geometryMenu;
	QMenu *m_aboutMenu;

	QToolBar *m_generalToolBar;
	QToolBar *m_optionsToolBar;
	QToolBar *m_geometryToolBar;

	QStatusBar *m_statusBar;

	QDockWidget *m_outlinerDock;
	TransferFunctionEditorDock *m_transferFunctionEditor;

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
	QAction *m_addVolumeAction;
	QAction *m_mipStateChanged;

	QSlider *m_tessellationSlider;
	QTreeView *m_outlinerTreeView;

	SceneController *m_sceneController;

private slots:
	void updateOutliner();
	void updateStatusBar(QString message) const;
};
