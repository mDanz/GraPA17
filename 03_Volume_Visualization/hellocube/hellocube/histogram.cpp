
#include "histogram.h"
#include <QByteArray>


Histogram::Histogram(QByteArray *byteData, int byteCount, int buckets)
{

	auto charData = reinterpret_cast<unsigned char*>(byteData->data());
	auto dataSize = byteData->size();

	m_domain = pow(256, byteCount);

	initializeMaxMinValues(byteCount, charData, dataSize);
	createHistogram(byteCount, charData, dataSize, buckets);
}

Histogram::~Histogram()
{
	delete[] m_histogram;
}

float Histogram::getMaxValue() const
{
	return m_maxValue;
}

float Histogram::getMinValue() const
{
	return m_minValue;
}

void Histogram::initializeMaxMinValues(int byteCount, unsigned char* charData, int dataSize)
{
	int value, maxV = 0, minV = m_domain;
	for (int i = 0; i < dataSize; i += byteCount)
	{
		value = 0;
		for (int b = byteCount - 1; b >= 0; b--)
		{
			value = (value << 8) | static_cast<int>(charData[i + b]);
		}

		if (value > maxV)
		{
			maxV = value;
		}
		if (value < minV)
		{
			minV = value;
		}
	}

	// normalize the max and min intensity values
	m_minValue = minV / static_cast<float>(m_domain);
	m_maxValue = maxV / static_cast<float>(m_domain);
}

void Histogram::createHistogram(int byteCount, unsigned char* charData, int dataSize, int buckets)
{
	m_buckets = buckets;
	m_histogram = new float[m_buckets];

	for (int i = 0; i < m_buckets; i++)
	{
		m_histogram[i] = 0;
	}

	uint value;
	int bucketId, maxBucket = 0;
	for (int i = 0; i < dataSize; i += byteCount)
	{
		value = 0;
		for (int b = byteCount - 1; b >= 0; b--)
		{
			value = (value << 8) | static_cast<int>(charData[i + b]);
		}

		//normalize
		bucketId = (value / static_cast<float>(m_domain) - m_minValue) / (m_maxValue - m_minValue) * m_buckets;
		m_histogram[bucketId]++;
	}
}
