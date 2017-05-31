
#include "transferfunction.h"
#include <qcolor.h>
#include <GL/gl.h>
#include "openglhelper.h"
#include <QFileDialog>

class QFile;

TransferFunction::TransferFunction()
	: TransferFunction(256)
{
}

TransferFunction::TransferFunction(int size)
{
	m_dataSize = size;
	m_function = new QColor[m_dataSize];
	m_textureName = GL_INVALID_VALUE;

	resetSelectedColorChannels();
	initializeTexture();
}

TransferFunction::~TransferFunction()
{
	delete[] m_function;
	OpenGLHelper::getGLFunc()->glDeleteTextures(1, &m_textureName);
}

void TransferFunction::resetSelectedColorChannels(bool isRedEnabled, bool isGreenEnabled, bool isBlueEnabled, bool isAlphaEnabled)
{
	QColor newColor;
	for (int i = 0; i < m_dataSize; i++)
	{
		newColor = QColor::fromHsv(0, 0, i * 255 / m_dataSize, i * 255 / m_dataSize);

		if (isRedEnabled) { m_function[i].setRedF(newColor.redF()); }
		if (isGreenEnabled) { m_function[i].setGreenF(newColor.greenF()); }
		if (isBlueEnabled) { m_function[i].setBlueF(newColor.blueF()); }
		if (isAlphaEnabled) { m_function[i].setAlphaF(newColor.alphaF()); }
	}

	emit transferFunctionChanged();
}

void TransferFunction::smoothSelectedColorChannels(bool isRedEnabled, bool isGreenEnabled, bool isBlueEnabled, bool isAlphaEnableda)
{
	QColor *smoothFunction = new QColor[m_dataSize];

	// pre-compute 2*sigma^2
	float sigma = sqrt(m_dataSize) / 3.f;
	sigma = 2 * sigma*sigma;

	// if we keep track of the sum of the weights (= wSum)	//todo understand this
	// we don't have to use the normalizing factor
	// 1/(2*pi*sigma^2) in every iteration
	float weight;
	float weightedSum;

	float red;
	float green;
	float blue;
	float alpha;

	for (int i = 0; i < m_dataSize; i++) {

		red = green = blue = alpha = 0;
		weightedSum = 0;

		// weigh all tf values by their distance to the
		// currently affected value
		for (int j = 0; j < m_dataSize; j++) {
			weight = exp(-(i - j)*(i - j) / sigma);
			red += m_function[j].redF() * weight;
			green += m_function[j].greenF() * weight;
			blue += m_function[j].blueF() * weight;
			alpha += m_function[j].alphaF() * weight;

			weightedSum += weight;
		}

		// normalize the new color by wSum
		smoothFunction[i] = m_function[i];
		if (isRedEnabled) { smoothFunction[i].setRedF(red / weightedSum); }
		if (isGreenEnabled) { smoothFunction[i].setGreenF(green / weightedSum); }
		if (isBlueEnabled) { smoothFunction[i].setBlueF(blue / weightedSum); }
		if (isAlphaEnableda) { smoothFunction[i].setAlphaF(alpha / weightedSum); }
	}

	delete[] m_function;
	m_function = smoothFunction;

	emit transferFunctionChanged();
}

int TransferFunction::getSize() const
{
	return m_dataSize;
}

QColor TransferFunction::getValue(int index) const
{
	if (index < 0 || index >= m_dataSize) 
	{
		qWarning() << "getValue: Index out of bound! Index: " << index;
		return QColor();
	}
	return m_function[index];
}

void TransferFunction::setValue(int index, QColor value)
{
	if (index < 0 || index > m_dataSize)
	{
		qWarning() << "setValue: Index out of bound! Index: " << index;
		return;
	}

	m_function[index] = value;
	updateTextureData();
	emit transferFunctionChanged();
}

GLuint TransferFunction::getTextureName() const
{
	return m_textureName;
}

void TransferFunction::save(QString filePath) const
{
	if (filePath.isEmpty())
	{
		qWarning() << "Could not save to empty string!";
		return;
	}

	QFile file(filePath);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		qWarning() << "Could not open file " << filePath << " for write!";
	}

	QDataStream outStream(&file);
	outStream << m_dataSize;
	for (int i = 0; i < m_dataSize; i++)
	{
		outStream << m_function[i];
	}

	file.close();
}

void TransferFunction::load(QString filePath)
{
	QFile file(filePath);
	if (!file.exists())
	{
		qWarning() << "File " << filePath << " does not exist!";
		return;
	}

	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Could not open file " << filePath << "!";
		return;
	}

	QDataStream inStream(&file);
	inStream >> m_dataSize;
	delete[] m_function;
	m_function = new QColor[m_dataSize];
	for (int i = 0; i < m_dataSize; i++)
	{
		inStream >> m_function[i];
	}

	file.close();
	updateTextureData();
	emit transferFunctionChanged();

}

void TransferFunction::initializeTexture()
{
	auto glFunc = OpenGLHelper::getGLFunc();

	glFunc->glGenTextures(1, &m_textureName);
	if (m_textureName == GL_INVALID_VALUE) 
	{
		qWarning() << "Could not create transfer function texture!";
		return;
	}

	glFunc->glBindTexture(GL_TEXTURE_1D, m_textureName);
	glFunc->glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glFunc->glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFunc->glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFunc->glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFunc->glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	updateTextureData();

	qInfo() << "Transfer function texture" << m_textureName << "created: " << OpenGLHelper::Error();
}

float* TransferFunction::getFunctionAsArray() const
{
	float *data = new float[m_dataSize * 4];
	for (int i = 0; i< m_dataSize * 4; i += 4)
	{
		data[i] = m_function[i / 4].redF();
		data[i+1] = m_function[i / 4].greenF();
		data[i+2] = m_function[i / 4].blueF();
		data[i+3] = m_function[i / 4].alphaF();
	}
	return data;
}

void TransferFunction::updateTextureData() const
{
	auto data = getFunctionAsArray();
	OpenGLHelper::getGLFunc()->glBindTexture(GL_TEXTURE_1D, m_textureName);
	OpenGLHelper::getGLFunc()->glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, m_dataSize, 0, GL_RGBA, GL_FLOAT, data);
	delete[] data;

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qWarning() << "Transfer Function Texture Data error: " << err;
	}
}
