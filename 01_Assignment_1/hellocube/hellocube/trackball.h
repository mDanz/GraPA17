#pragma once

#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>
#include <QtCore/QTime>

class TrackBall
{
public:
	enum TrackMode
	{
		Plane,
		Sphere,
	};
	
	TrackBall(TrackMode mode = Sphere);
	TrackBall(float angularVelocity, const QVector3D &axis, TrackMode mode = Sphere);

	void move(const QPointF &pos, const QQuaternion &transformation);
	void release(const QPointF &pos, const QQuaternion &transformation);
private:
	QQuaternion m_rotation;
	QVector3D m_axis;
	float m_angularVelocity;

	QPointF m_lastPos;
	QTime m_lastTime;
	bool m_pause;
	bool m_pressed;
	TrackMode m_mode;

	void calculatePlaneMove(const QPointF &pos, const QQuaternion &transformation, const int msecs);
	void calculateSphereMove(const QPointF &pos, const QQuaternion &transformation, const int msecs);
	void calculate3DPos(QVector3D &pos3D);
};

