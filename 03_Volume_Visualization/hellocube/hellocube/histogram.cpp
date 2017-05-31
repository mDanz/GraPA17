
#include "histogram.h"
#include <QByteArray>


Histogram::Histogram(QByteArray *byteData, int byteCount, int buckets)
{

	m_charData = reinterpret_cast<unsigned char*>(byteData->data());
	m_dataSize = byteData->size();

	m_domain = pow(256, byteCount);
	m_byteCount = byteCount;

	initializeMaxMinValues();
	createHistogram(buckets);
}

Histogram::~Histogram()
{
	delete[] m_histogram;
	delete[] m_charData;
}

float Histogram::getMaxValue() const
{
	return m_maxValue;
}

float Histogram::getMinValue() const
{
	return m_minValue;
}

float* Histogram::getHistogramData(int buckets)
{
	if (m_buckets == buckets)
	{
		return m_histogram;
	}
	createHistogram(buckets);
	return m_histogram;
}

void Histogram::initializeMaxMinValues()
{
	int value, maxV = 0, minV = m_domain;
	for (int i = 0; i < m_dataSize; i += m_byteCount)
	{
		value = 0;
		for (int b = m_byteCount - 1; b >= 0; b--)
		{
			value = (value << 8) | static_cast<int>(m_charData[i + b]);
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

void Histogram::createHistogram(int buckets)
{
	m_buckets = buckets;
	m_histogram = new float[m_buckets];

	for (int i = 0; i < m_buckets; i++)
	{
		m_histogram[i] = 0;
	}

	uint value = 0;
	int bucketId = 0;
	int maxBucketValue = 0;
	for (int i = 0; i < m_dataSize; i += m_byteCount)
	{
		value = 0;
		for (int b = m_byteCount - 1; b >= 0; b--)
		{
			value = (value << 8) | static_cast<int>(m_charData[i + b]);
		}

		bucketId = (value / static_cast<float>(m_domain) - m_minValue) / (m_maxValue - m_minValue) * m_buckets;
		m_histogram[bucketId]++;

		if (m_histogram[bucketId] > maxBucketValue) 
		{
			maxBucketValue = m_histogram[bucketId];
		}
	}

	normalizeBucketsLogarithmic(maxBucketValue);
}

void Histogram::normalizeBucketsLogarithmic(int& maxBucketValue) const
{
	maxBucketValue = log(maxBucketValue);
	for (int i = 0; i < m_buckets; i++)
	{
		if (m_histogram[i] > 0)
		{
			m_histogram[i] = log(m_histogram[i]) / maxBucketValue;
		}
	}
}
