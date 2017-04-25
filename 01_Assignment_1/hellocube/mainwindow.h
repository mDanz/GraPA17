#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMenuBar *menuBar;

    QMenu *fileMenu;
    QMenu *shadingMenu;

    QActionGroup *shadingActionGroup;

    QAction *exitAction;
    QAction *noneShadingAction;
    QAction *flatShadingAction;
    QAction *gouraudShadingAction;
    QAction *phongShadingAction;
};

#endif // MAINWINDOW_H
