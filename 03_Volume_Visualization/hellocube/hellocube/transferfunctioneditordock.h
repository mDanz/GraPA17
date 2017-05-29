#pragma once
#include <QDockWidget>
#include "transferfunctioncanvas.h"
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include "volumemodel.h"


class TransferFunction;

class TransferFunctionEditorDock : public QDockWidget
{
public:
	explicit TransferFunctionEditorDock(QWidget *parent = nullptr, TransferFunction *transferFunc = nullptr);
	~TransferFunctionEditorDock();

private:
	void initializeCanvas();
	void initializeCheckBoxes();
	void initializeButtons();

	TransferFunction *m_transferFunc;
	VolumeModel *m_volume;
	
	TransferFunctionCanvas *m_canvas;
	QGridLayout *m_layout;

	QCheckBox* m_redBox;
	QCheckBox* m_greenBox;
	QCheckBox* m_blueBox;
	QCheckBox* m_alphaBox;
	QPushButton* m_smoothButton;
	QPushButton* m_resetButton;
};
