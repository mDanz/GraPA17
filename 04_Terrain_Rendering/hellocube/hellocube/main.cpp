#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow window;
	window.resize(1200, 800);
	window.show();
	return a.exec();
}
