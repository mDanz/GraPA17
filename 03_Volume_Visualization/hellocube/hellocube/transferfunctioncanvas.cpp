
#include "transferfunctioncanvas.h"

TransferFunctionCanvas::TransferFunctionCanvas(QWidget* parent, int width, int height)
	: QWidget(parent)
{
	setFixedSize(width, height);
}

TransferFunctionCanvas::~TransferFunctionCanvas()
{
}

void TransferFunctionCanvas::renderTransferFunction(TransferFunction* transferFunc, VolumeModel* volume)
{
	m_transferFunc = transferFunc;
	m_volume = volume;
}

void TransferFunctionCanvas::setAffectedColorChannels(bool red, bool green, bool blue, bool alpha)
{
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
}
