#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello Cube");

    initializeMenuBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::showAboutBox()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Marcel Danz.");
    msgBox.exec();
}

void MainWindow::initializeMenuBar()
{
    menuBar = new QMenuBar();

    initializeFileMenu();
    initializeShadingMenu();
    initializeAboutMenu();

    setMenuBar(menuBar);
}

void MainWindow::initializeFileMenu()
{
    fileMenu = new QMenu("&File");

    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    fileMenu->addAction(exitAction);

    menuBar->addMenu(fileMenu);

     connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::initializeShadingMenu()
{
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

    noneShadingAction->setIcon(QIcon(":/img/wireframe.png"));
    flatShadingAction->setIcon(QIcon(":/img/flat.png"));
    gouraudShadingAction->setIcon(QIcon(":/img/gouraud.png"));
    phongShadingAction->setIcon(QIcon(":/img/phong.png"));

    shadingActionGroup->addAction(noneShadingAction);
    shadingActionGroup->addAction(flatShadingAction);
    shadingActionGroup->addAction(gouraudShadingAction);
    shadingActionGroup->addAction(phongShadingAction);
    flatShadingAction->setChecked(true);

    menuBar->addMenu(shadingMenu);
}

void MainWindow::initializeAboutMenu()
{
    aboutMenu = new QMenu("&About");
    aboutAction = new QAction("&About", aboutMenu);
    aboutMenu->addAction(aboutAction);
    menuBar->addMenu(aboutMenu);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
}







