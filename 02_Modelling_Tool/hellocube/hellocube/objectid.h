#pragma once
#include <qcolor.h>


class ObjectID
{
public:
	explicit ObjectID(int m_id);

	int getId() const;
	QColor getIdAsColor() const;
	static int getIdFromColor(QColor* color);
	void setId(int id);
	void setId(QColor* color);

private:
	int m_id;

	static int color2Id(QColor* color);
	static QColor id2Color(int id);
};
