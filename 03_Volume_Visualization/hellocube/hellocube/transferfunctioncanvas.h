#pragma once
#include <QWidget>
#include "transferfunction.h"
#include "volumemodel.h"

class TransferFunctionCanvas : public QWidget
{
	
public:
	explicit TransferFunctionCanvas(QWidget *parent = nullptr, int width = 256, int height = 256);
	~TransferFunctionCanvas();

	void renderTransferFunction(TransferFunction* transferFunc, VolumeModel* volume);
	void setAffectedColorChannels(bool red, bool green, bool blue, bool alpha);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	void changeTransferFunction(int x, int y);

	bool m_red;
	bool m_green;
	bool m_blue;
	bool m_alpha;

	TransferFunction *m_transferFunc;
	VolumeModel *m_volume;
};

