
#include "openglwidget.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QQuaternion>
#include <QMatrix4x4>

# define M_PI           3.14159265358979323846

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), QOpenGLFunctions_4_4_Compatibility()
	, m_tesselation(0)
	, m_wheelDelta(0)
{

	m_lastPos = new QPoint();
	m_dragTranslation = new GLfloat[2]{0, 0};
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

OpenGLWidget::~OpenGLWidget()
{
	
}

QSize OpenGLWidget::minimumSizeHint() const
{
	return QSize(1200, 700);
}

QSize OpenGLWidget::sizeHint() const
{
	return QSize(2000, 1500);
}

void OpenGLWidget::wireframeShading()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	update();
}

void OpenGLWidget::flatShading()
{
	glShadeModel(GL_FLAT);
	update();
}

void OpenGLWidget::gouraudShading()
{
	glShadeModel(GL_SMOOTH);
	update();
}

void OpenGLWidget::phongShading()
{
	//todo
	update();
}

void OpenGLWidget::setTesselation(int t)
{
	m_tesselation = t;
	update();
}

void OpenGLWidget::resetCamera()
{
	m_tesselation = 0;
	m_wheelDelta = 0;
	m_dragTranslation = new GLfloat[2]{0, 0};
	m_lastPos = new QPoint();
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(m_trackBall->getMode());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	update();
}


void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_LIGHTING);
	const GLfloat lightPos[4] = { 0.5f, 0.0f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
}

static const float cube[6][4][3] = {
	{ {+.5, -.5, +.5}, {+.5, -.5, -.5}, {+.5, +.5, -.5}, {+.5, +.5, +.5} },
	{ {-.5, -.5, -.5}, {-.5, -.5, +.5}, {-.5, +.5, +.5}, {-.5, +.5, -.5} },
	{ {+.5, -.5, -.5}, {-.5, -.5, -.5}, {-.5, +.5, -.5}, {+.5, +.5, -.5} },
	{ {-.5, -.5, +.5}, {+.5, -.5, +.5}, {+.5, +.5, +.5}, {-.5, +.5, +.5} },
	{ {-.5, -.5, -.5}, {+.5, -.5, -.5}, {+.5, -.5, +.5}, {-.5, -.5, +.5} },
	{ {-.5, +.5, +.5}, {+.5, +.5, +.5}, {+.5, +.5, -.5}, {-.5, +.5, -.5} }
};

static const QVector3D normals[6] = {
	QVector3D(1, 0, 0),
	QVector3D(-1, 0, 0),
	QVector3D(0, 0, -1),
	QVector3D(0, 0, 1),
	QVector3D(0, -1, 0),
	QVector3D(0, 1, 0)
};

static const float faceColors[6][3]{
	{ 1, 0, 0 },
	{ 0, 1, 1 },
	{ 1, 0, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 0 },
	{ 0, 1, 0 }
};

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, m_wheelDelta - 5);	//zoom
	glTranslatef(m_dragTranslation[0], -m_dragTranslation[1], 0);	//mouse dragging

	auto dragRotation = QMatrix4x4();
	dragRotation.rotate(m_dragRotation);
	glMultMatrixf(dragRotation.constData());	//mouse rotation

	for (auto i = 0; i < 6; i++)
	{
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColors[i]);
		glNormal3f(normals[i].x(), normals[i].y(), normals[i].z());
		for (auto j = 0; j < 4; j++)
		{
			glVertex3f(cube[i][j][0], cube[i][j][1], cube[i][j][2]);
		}
		glEnd();
	}
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	GLdouble aspect = GLfloat(width) / (height ? height : 1);
	perspective(m_fov, aspect, m_zNear, m_zFar);
	
	glMatrixMode(GL_MODELVIEW);

	update();
}

QPointF OpenGLWidget::pixelPosToViewPos(const QPointF &pos) const
{

	auto x = 2.0 * float(pos.x()) / width() - 1.0;
	auto y = 1.0 - 2.0 * float(pos.y()) / height();
	return QPointF(x, y);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	auto dx = event->x() - m_lastPos->x();
	auto dy = event->y() - m_lastPos->y();

	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->move(pixelPosToViewPos(event->pos()), m_trackBall->rotation().conjugate());
		m_dragRotation = m_trackBall->rotation();
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_trackBall->release(pixelPosToViewPos(event->pos()), m_trackBall->rotation().conjugate());
		m_dragTranslation[0] = dx * m_damping;
		m_dragTranslation[1] = dy * m_damping;
	}
	else
	{
		m_trackBall->release(pixelPosToViewPos(event->pos()), m_trackBall->rotation().conjugate());
	}

	update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	m_lastPos = new QPoint(event->pos());
	
	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->push(pixelPosToViewPos(event->pos()));
	}

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_trackBall->release(pixelPosToViewPos(event->pos()), m_trackBall->rotation().conjugate());
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	m_wheelDelta += event->delta() * m_damping;
	update();
}


void OpenGLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble left, right, bottom, top;

	top = zNear * tan(fovy * M_PI / 360.0f);
	bottom = -top;
	left = bottom * aspect;
	right = top * aspect;

	glFrustum(left, right, bottom, top, zNear, zFar);
}






