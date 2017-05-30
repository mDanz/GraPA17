
#include "transferfunction.h"
#include <qcolor.h>

int TransferFunction::getSize() const
{
	return m_dataSize;
}

QColor TransferFunction::getValue(int index) const
{
	if (index < m_dataSize) 
	{
		return m_function[index];
	}
}

void TransferFunction::setValue(int index, QColor value)
{
	if (index < 0 || index > m_dataSize)
	{
		qWarning() << "Index out of boundary!";
		return;
	}

	m_function[index] = value;
	emit transferFunctionChanged();
}
