#pragma once
#include <qobjectdefs.h>
#include <QObject>
#include <qstring.h>


class TransferFunction : public QObject
{
	Q_OBJECT

public:
	TransferFunction();
	~TransferFunction();

	void resetSelectedColorChannels(int type = 0, bool r = true, bool g = true, bool b = true, bool a = true);
	void smoothSelectedColorChannels(bool r = true, bool g = true, bool b = true, bool a = true);

	int getSize() const;
	QColor getValue(int index) const;
	void setValue(int index, QColor value);

signals:
	void transferFunctionChanged();

private:
	int m_dataSize;
	QColor *m_function;
};
