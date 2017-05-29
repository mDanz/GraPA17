#pragma once


class TransferFunction : public QObject
{
	Q_OBJECT

public:
	TransferFunction();
	~TransferFunction();

	void resetSelectedColorChannels(int type = 0, bool r = true, bool g = true, bool b = true, bool a = true);
	void smoothSelectedColorChannels(bool r = true, bool g = true, bool b = true, bool a = true);
};