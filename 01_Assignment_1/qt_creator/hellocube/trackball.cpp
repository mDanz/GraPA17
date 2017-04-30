

#include "trackball.h"
#include <qline.h>

# define M_PI           3.14159265358979323846

TrackBall::TrackBall(TrackMode mode)
{
}

TrackBall::TrackBall(float angularVelocity, const QVector3D& axis, TrackMode mode)
{
}

void TrackBall::move(const QPointF& pos, const QQuaternion& transformation)
{
	if (!m_pressed)
	{
		return;
	}

	auto currentTime = QTime::currentTime();
	auto msecs = m_lastTime.msecsTo(currentTime);
	if (msecs <= 20)
	{
		return;
	}

	switch (m_mode)
	{
	case Plane:
		calculatePlaneMove(pos, transformation, msecs);
		break;
	case Sphere:
		calculateSphereMove(pos, transformation, msecs);
		break;
	}

	m_lastPos = pos;
	m_lastTime = currentTime;
}

void TrackBall::release(const QPointF &pos, const QQuaternion &transformation)
{
	move(pos, transformation);
	m_pressed = false;
}

void TrackBall::calculatePlaneMove(const QPointF &pos, const QQuaternion &transformation, const int msecs)
{
	auto delta = QLineF(m_lastPos, pos);
	m_angularVelocity = 180 * delta.length() / (M_PI * msecs);
	m_axis = QVector3D(-delta.dy(), delta.dx(), 0.0f).normalized();
	m_axis = transformation.rotatedVector(m_axis);
	m_rotation = QQuaternion::fromAxisAndAngle(m_axis, 180 / M_PI * delta.length()) * m_rotation;
}

void TrackBall::calculateSphereMove(const QPointF &pos, const QQuaternion &transformation, const int msecs)
{
	auto lastPos3D = QVector3D(m_lastPos.x(), m_lastPos.y(), 0.0f);
	calculate3DPos(lastPos3D);
	auto currentPos3D = QVector3D(pos.x(), pos.y(), 0.0f);
	calculate3DPos(currentPos3D);

	m_axis = QVector3D::crossProduct(lastPos3D, currentPos3D);
	float angle = 180 / M_PI * std::asin(std::sqrt(QVector3D::dotProduct(m_axis, m_axis)));
	m_axis.normalize();
	m_axis = transformation.rotatedVector(m_axis);
	m_angularVelocity = angle / msecs;

	m_rotation = QQuaternion::fromAxisAndAngle(m_axis, angle) * m_rotation;
}

void TrackBall::calculate3DPos(QVector3D &pos3D)
{

	float sqrZ = 1 - QVector3D::dotProduct(pos3D, pos3D);
	if (sqrZ > 0)
	{
		pos3D.setZ(std::sqrt(sqrZ));
	}
	else
	{
		pos3D.normalize();
	}
}
