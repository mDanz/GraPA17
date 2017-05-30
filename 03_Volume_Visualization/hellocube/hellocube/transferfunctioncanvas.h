#pragma once
#include <QWidget>
#include "transferfunction.h"
#include "volumemodel.h"

class TransferFunctionCanvas : public QWidget
{
	
public:
	explicit TransferFunctionCanvas(QWidget *parent = nullptr, int width = 256, int height = 256);
	~TransferFunctionCanvas();

	void renderTransferFunction(VolumeModel* volume);
	void setAffectedColorChannels(bool red, bool green, bool blue, bool alpha);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	void paintHistogram(QPainter& paint) const;
	void paintTransferFunction(QPainter& paint) const;
	void changeTransferFunction(QPoint pos);
	void calculateNewValuesInSector(TransferFunction* transferFunction, int startX, int endX, float leftValue, float rightValue) const;
	static float clamp(float value, float lower, float upper);

	bool m_isRedEnabled;
	bool m_isGreenEnabled;
	bool m_isBlueEnabled;
	bool m_isAlphaEnabled;

	QPoint m_previousMousePos;
	VolumeModel *m_volume;
};

