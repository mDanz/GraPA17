
#include "transferfunctioncanvas.h"
#include <QPainter>
#include <QWheelEvent>

TransferFunctionCanvas::TransferFunctionCanvas(QWidget* parent, int width, int height)
	: QWidget(parent)
{
	setFixedSize(width, height);
}

TransferFunctionCanvas::~TransferFunctionCanvas()
{
	m_volume = nullptr;
}

void TransferFunctionCanvas::reRenderTransferFunction(VolumeModel* volume)
{
	m_volume = volume;
	update();
}

void TransferFunctionCanvas::setAffectedColorChannels(bool red, bool green, bool blue, bool alpha)
{
	m_isRedEnabled = red;
	m_isGreenEnabled = green;
	m_isBlueEnabled = blue;
	m_isAlphaEnabled = alpha;
}

void TransferFunctionCanvas::paintEvent(QPaintEvent* event)
{
	if (!m_volume || !m_volume->getTransferFunction())
	{
		return;
	}

	QPainter paint(this);
	paint.fillRect(0, 0, width(), height(), QBrush(QColor::fromHsv(0, 0, 220)));

	paintHistogram(paint);
	paintTransferFunction(paint);
}

void TransferFunctionCanvas::mousePressEvent(QMouseEvent* event)
{
	changeTransferFunction(event->pos());
}

void TransferFunctionCanvas::mouseMoveEvent(QMouseEvent* event)
{
	changeTransferFunction(event->pos());
}

void TransferFunctionCanvas::paintHistogram(QPainter& paint) const
{
	paint.setPen(QColor::fromHsv(0, 0, 100));
	float *histogram = m_volume->getHistogram()->getHistogramData(width());
	for (int i = 0; i<m_volume->getTransferFunction()->getSize(); i++)
	{
		auto h = height() - histogram[i] * height();
		paint.drawLine(i, height(), i, h);
	}
}

void TransferFunctionCanvas::paintTransferFunction(QPainter& paint) const
{
	auto transferFunction = m_volume->getTransferFunction();

	QPointF previousRed(0, height() - transferFunction->getValue(0).redF() * height());
	QPointF previousGreen(0, height() - transferFunction->getValue(0).greenF() * height());
	QPointF previousBlue(0, height() - transferFunction->getValue(0).blueF() * height());
	QPointF previousAlpha(0, height() - transferFunction->getValue(0).alphaF() * height());
	QPointF redPos(0, 0);
	QPointF greenPos(0, 0);
	QPointF bluePos(0, 0);
	QPointF alphaPos(0, 0);
	
	QColor color;

	for (int i = 0; i < transferFunction->getSize(); i++) 
	{
		color = transferFunction->getValue(i);

		redPos.setX(i * width() / transferFunction->getSize());
		redPos.setY(height() - color.redF() * height());
		paint.setPen(Qt::red);
		paint.drawLine(previousRed, redPos);

		greenPos.setX(i * width() / transferFunction->getSize());
		greenPos.setY(height() - color.greenF() * height());
		paint.setPen(Qt::green);
		paint.drawLine(previousGreen, greenPos);

		bluePos.setX(i * width() / transferFunction->getSize());
		bluePos.setY(height() - color.blueF() * height());
		paint.setPen(Qt::blue);
		paint.drawLine(previousBlue, bluePos);

		alphaPos.setX(i * width() / transferFunction->getSize());
		alphaPos.setY(height() - color.alphaF() * height());
		paint.setPen(Qt::black);
		paint.drawLine(previousAlpha, alphaPos);

		previousRed = redPos;
		previousGreen = greenPos;
		previousBlue = bluePos;
		previousAlpha = alphaPos;
	}
}

void TransferFunctionCanvas::changeTransferFunction(QPoint pos)
{
	if (!m_volume || !m_volume->getTransferFunction())
	{
		return;
	}

	auto transferFunction = m_volume->getTransferFunction();

	int startX = m_previousMousePos.x() * transferFunction->getSize() / width(); //todo rework
	int endX = pos.x() * transferFunction->getSize() / width();

	float leftValue;
	float rightValue;
	if (endX < startX) 
	{
		int temp = endX;
		endX = startX;
		startX = temp;

		rightValue = 1.f - m_previousMousePos.y() / static_cast<float>(height());
		leftValue = 1.f - pos.y() / static_cast<float>(height());
	}
	else 
	{
		rightValue = 1.f - pos.y() / static_cast<float>(height());
		leftValue = 1.f - m_previousMousePos.y() / static_cast<float>(height());
	}

	calculateNewValuesInSector(transferFunction, startX, endX, leftValue, rightValue);

	m_previousMousePos = pos;
}

void TransferFunctionCanvas::calculateNewValuesInSector(TransferFunction* transferFunction, int startX, int endX, float leftValue, float rightValue) const
{
	float distance = endX - startX + 1;
	float value;
	for (int i = startX; i <= endX; i++)
	{
		if (i < 0 || i >= transferFunction->getSize())
		{
			continue;
		}

		value = leftValue * (1.f - (i - startX) / distance) + rightValue * (i - startX) / distance;
		value = clamp(value, 0.0f, 1.0f);

		QColor colorValue = transferFunction->getValue(i);
		if (m_isRedEnabled) { colorValue.setRedF(value); }
		if (m_isGreenEnabled) { colorValue.setGreenF(value); }
		if (m_isBlueEnabled) { colorValue.setBlueF(value); }
		if (m_isAlphaEnabled) { colorValue.setAlphaF(value); }
		transferFunction->setValue(i, colorValue);
	}
}

float TransferFunctionCanvas::clamp(float value, float lower, float upper)
{
	if (value < lower)
	{
		return lower;
	}
	if (value > upper)
	{
		return upper;
	}
	return value;
}
