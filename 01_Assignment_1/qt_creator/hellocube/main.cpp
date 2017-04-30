#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow window;
	window.resize(640, 480);
	window.show();
	return a.exec();
}
