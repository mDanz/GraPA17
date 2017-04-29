#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test.h"

class test : public QMainWindow
{
	Q_OBJECT

public:
	test(QWidget *parent = Q_NULLPTR);

private:
	Ui::testClass ui;
};
