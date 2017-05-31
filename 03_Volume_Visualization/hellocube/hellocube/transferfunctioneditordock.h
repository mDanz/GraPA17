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
	Q_OBJECT

public:
	explicit TransferFunctionEditorDock(QString title = "", QWidget *parent = nullptr);
	~TransferFunctionEditorDock();

public slots:
	void updateVolume(VolumeModel *volume);

private:
	void initializeCanvas();
	void initializeCheckBoxes();
	void initializeButtons();

	VolumeModel *m_volume;

	TransferFunctionCanvas *m_canvas;
	QGridLayout *m_layout;

	QCheckBox* m_redBox;
	QCheckBox* m_greenBox;
	QCheckBox* m_blueBox;
	QCheckBox* m_alphaBox;
	QPushButton* m_smoothButton;
	QPushButton* m_resetButton;
	QPushButton* m_loadButton;
	QPushButton* m_saveButton;

private slots:
	void updateColorChannelBoxes() const;
	void updateCanvas() const;
	void smoothTransferFunction() const;
	void resetTransferFunction() const;
	void saveTransferFunction();
	void loadTransferFunction();
};
