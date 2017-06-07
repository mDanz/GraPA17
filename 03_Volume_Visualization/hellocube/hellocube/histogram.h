#pragma once


class QByteArray;

class Histogram
{
public:
	Histogram(QByteArray *byteData, int byteCount, int buckets);
	~Histogram();

	float getMaxValue() const;
	float getMinValue() const;
	float* getHistogramData(int buckets);


private:
	void initializeMaxMinValues();
	void createHistogram(int buckets);
	void normalizeBucketsLogarithmic(int& maxBucketValue) const;

	unsigned char* m_charData;
	float *m_histogram;
	int m_buckets;
	int m_byteCount;
	int m_dataSize;
	float m_maxValue;
	float m_minValue;
	int m_domain;
};

