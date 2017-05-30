#pragma once
#include <qobjectdefs.h>
#include <QObject>
#include <qstring.h>
#include <qopengl.h>


class TransferFunction : public QObject
{
	Q_OBJECT

public:
	TransferFunction();
	explicit TransferFunction(int size);
	~TransferFunction();

	void resetSelectedColorChannels(bool isRedEnabled = true, bool isGreenEnabled = true, bool isBlueEnabled = true, bool isAlphaEnabled = true);
	void smoothSelectedColorChannels(bool isRedEnabled = true, bool isGreenEnabled = true, bool isBlueEnabled = true, bool isAlphaEnableda = true);

	int getSize() const;
	QColor getValue(int index) const;
	void setValue(int index, QColor value);
	GLuint getTextureName() const;

signals:
	void transferFunctionChanged();

private:

	void initializeTexture();
	float* getFunctionAsArray() const;
	void updateTextureData() const;

	int m_dataSize;
	QColor *m_function;
	GLuint m_textureName;
};
