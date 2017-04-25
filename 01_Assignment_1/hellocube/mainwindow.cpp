#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello Cube");

    menuBar = new QMenuBar();

    fileMenu = new QMenu("&File");

    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    fileMenu->addAction(exitAction);

    menuBar->addMenu(fileMenu);


    shadingMenu = new QMenu("&Shading");
    shadingActionGroup = new QActionGroup(this);

    noneShadingAction = new QAction("&None (Wireframe)", shadingMenu);
    flatShadingAction = new QAction("&Flat", shadingMenu);
    gouraudShadingAction = new QAction("&Gouraud)", shadingMenu);
    phongShadingAction = new QAction("&Phong)", shadingMenu);

    noneShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    flatShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    gouraudShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    phongShadingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));

    shadingMenu->addAction(noneShadingAction);
    shadingMenu->addAction(flatShadingAction);
    shadingMenu->addAction(gouraudShadingAction);
    shadingMenu->addAction(phongShadingAction);

    noneShadingAction->setCheckable(true);
    flatShadingAction->setCheckable(true);
    gouraudShadingAction->setCheckable(true);
    phongShadingAction->setCheckable(true);

    shadingActionGroup->addAction(noneShadingAction);
    shadingActionGroup->addAction(flatShadingAction);
    shadingActionGroup->addAction(gouraudShadingAction);
    shadingActionGroup->addAction(phongShadingAction);
    flatShadingAction->setChecked(true);

    menuBar->addMenu(shadingMenu);

    setMenuBar(menuBar);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

}

MainWindow::~MainWindow()
{

}
