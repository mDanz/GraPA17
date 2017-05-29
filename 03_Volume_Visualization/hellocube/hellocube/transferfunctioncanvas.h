#pragma once
#include <QWidget>
#include "transferfunction.h"
#include "volumemodel.h"

class TransferFunctionCanvas : public QWidget
{
	
public:
	TransferFunctionCanvas(QWidget *parent = nullptr);
	~TransferFunctionCanvas();

	void renderTransferFunction(TransferFunction* transferFunc, VolumeModel* volume);
};
