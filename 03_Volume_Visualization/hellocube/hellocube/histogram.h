#pragma once


class QByteArray;

class Histogram
{
public:
	Histogram(QByteArray *byteData, int byteCount, int buckets);
	~Histogram();

	float getMaxValue() const;
	float getMinValue() const;

private:
	void initializeMaxMinValues(int byteCount, unsigned char* charData, int dataSize);
	void createHistogram(int byteCount, unsigned char* charData, int dataSize, int buckets);

	float *m_histogram;
	int m_buckets;
	float m_maxValue;
	float m_minValue;
	int m_domain;
};

