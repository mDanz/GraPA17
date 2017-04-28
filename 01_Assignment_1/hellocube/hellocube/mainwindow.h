#pragma once

#include <QtWidgets>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

	public slots:
	void showAboutBox();

private:
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

	void initializeMenuBar();
	void initializeFileMenu();
	void initializeShadingMenu();
	void initializeShadingActionGroup();
	void initializeAboutMenu();
	void initializeToolBar();
	void initializeStatusBar();

	Ui::MainWindowClass ui;
};
