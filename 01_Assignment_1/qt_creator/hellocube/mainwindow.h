#pragma once

#include <QtWidgets/QMainWindow>
#include <QActionGroup>
#include <QSlider>
//#include "ui_mainwindow.h"
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
    //Ui::MainWindowClass ui;
	OpenGLWidget *openGLWidget;

	QMenuBar *menuBar;

	QMenu *fileMenu;
	QMenu *shadingMenu;
	QMenu *aboutMenu;

	QToolBar *shadingToolBar;

	QStatusBar *statusBar;

	QActionGroup *shadingActionGroup;

	QAction *exitAction;
	QAction *noneShadingAction;
	QAction *flatShadingAction;
	QAction *gouraudShadingAction;
	QAction *phongShadingAction;
	QAction *aboutAction;
	QAction *resetCameraAction;

	QSlider *tesselationSlider;

	void initializeMenuBar();
	void initializeFileMenu();
	void initializeShadingMenu();
	void initializeShadingActionGroup();
	void initializeAboutMenu();
	void initializeToolBar();
	void initializeStatusBar();
	QSlider* createSlider();
};
