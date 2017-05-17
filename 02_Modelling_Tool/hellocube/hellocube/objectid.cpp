#include "objectid.h"

ObjectID::ObjectID(int m_id)
: m_id(m_id)
{

}

int ObjectID::getId() const
{
	return m_id;
}

QColor ObjectID::getIdAsColor() const
{
	return id2Color(m_id);
}

int ObjectID::getIdFromColor(QColor* color)
{
	return color->red() + color->green() * 256 + color->blue() * 256 * 256;
}

void ObjectID::setId(int id)
{
	m_id = id;
}

void ObjectID::setId(QColor* color)
{
	m_id = color2Id(color);
}

int ObjectID::color2Id(QColor* color)
{
	return color->red() + color->green() * 256 + color->blue() * 256 * 256;
}

QColor ObjectID::id2Color(int id)
{
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	return QColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0);
}


