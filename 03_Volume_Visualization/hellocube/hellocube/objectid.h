#pragma once
#include <qcolor.h>


class QVector3D;

class ObjectID
{
public:
	explicit ObjectID(int m_id);

	int getId() const;
	QVector3D getIdAsColor() const;
	static int getIdFromColor(QVector3D color);
	void setId(int id);
	void setId(QVector3D* color);

private:
	int m_id;

	static int color2Id(QVector3D* color);
	static QVector3D id2Color(int id);
};
