#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showAboutBox();

private:
    QMenuBar *menuBar;

    QMenu *fileMenu;
    QMenu *shadingMenu;
    QMenu *aboutMenu;

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
    void initializeAboutMenu();
    void initializeToolBar();
    void initializeStatusBar();
};

#endif // MAINWINDOW_H
