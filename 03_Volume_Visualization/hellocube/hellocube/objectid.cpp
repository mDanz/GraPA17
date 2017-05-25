#include "objectid.h"
#include <QVector3D>

int ObjectID::s_nextId = 0;

ObjectID* ObjectID::createID()
{
	return new ObjectID(s_nextId++);
}

ObjectID::ObjectID(int m_id)
: m_id(m_id)
{

}

int ObjectID::getId() const
{
	return m_id;
}

QVector3D ObjectID::getIdAsColor() const
{
	return id2Color(m_id);
}

int ObjectID::getIdFromColor(QVector3D color)
{
	return color2Id(&color);
}

void ObjectID::setId(int id)
{
	m_id = id;
}

void ObjectID::setId(QVector3D* color)
{
	m_id = color2Id(color);
}

int ObjectID::color2Id(QVector3D* color)
{
	return color->x() + color->y() * 256 + color->z() * 256 * 256;
}

QVector3D ObjectID::id2Color(int id)
{
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	return QVector3D(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f);
}


