#pragma once
#include <qcolor.h>


class QVector3D;

class ObjectID
{
public:
	static ObjectID* createID();

	int getId() const;
	QVector3D getIdAsColor() const;
	static int getIdFromColor(QVector3D color);
	void setId(int id);
	void setId(QVector3D* color);

private:
	static int s_nextId;

	explicit ObjectID(int m_id);
	int m_id;

	static int color2Id(QVector3D* color);
	static QVector3D id2Color(int id);
};
