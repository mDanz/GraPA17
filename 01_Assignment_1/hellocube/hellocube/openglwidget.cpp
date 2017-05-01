
#include "openglwidget.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLFunctions_4_5_Core>

# define M_PI           3.14159265358979323846

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), QOpenGLFunctions_4_4_Compatibility()
	, m_tesselation(0)
	, m_wheelDelta(0)
{

	m_lastPos = new QPoint();
	m_dragTranslation = new QVector2D();
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Plane);

	faceColors[0] = Qt::red;
	faceColors[1] = Qt::green;
	faceColors[2] = Qt::blue;
	faceColors[3] = Qt::cyan;
	faceColors[4] = Qt::magenta;
	faceColors[5] = Qt::yellow;


	//m_context = new QOpenGLContext;
	//m_context->create();

	////todo
	//QSurfaceFormat format;
	//format.setDepthBufferSize(24);
	//format.setStencilBufferSize(8);

	//format.setMajorVersion(4); //whatever version
	//format.setMinorVersion(5); //
	//format.setProfile(QSurfaceFormat::CompatibilityProfile);
	//

	/////*m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprogile"));
	////m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
	////if (m_transparent)
	////{
	////	format.setAlphaBufferSize(8);
	////	
	////}*/

	//QSurfaceFormat::setDefaultFormat(format);
	//setFormat(format);
}

OpenGLWidget::~OpenGLWidget()
{
	
}

QSize OpenGLWidget::minimumSizeHint() const
{
	return QSize(400, 400);
}

QSize OpenGLWidget::sizeHint() const
{
	return QSize(800, 800);
}

void OpenGLWidget::wireframeShading()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	update();
}

void OpenGLWidget::flatShading()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	m_dragTranslation = new QVector2D();
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
	glClearColor(.5f, .5f, .5f, 1);
	glShadeModel(GL_FLAT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	const GLfloat lightPos[4] = { 0.5f, 0.0f, 0.2f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

static const QVector3D cube[6][4] = {
	{ QVector3D(+.5, -.5, +.5), QVector3D(+.5, -.5, -.5), QVector3D(+.5, +.5, -.5), QVector3D(+.5, +.5, +.5) },
	{ QVector3D(-.5, -.5, -.5), QVector3D(-.5, -.5, +.5), QVector3D(-.5, +.5, +.5), QVector3D(-.5, +.5, -.5) },
	{ QVector3D(+.5, -.5, -.5), QVector3D(-.5, -.5, -.5), QVector3D(-.5, +.5, -.5), QVector3D(+.5, +.5, -.5) },
	{ QVector3D(-.5, -.5, +.5), QVector3D(+.5, -.5, +.5), QVector3D(+.5, +.5, +.5), QVector3D(-.5, +.5, +.5) },
	{ QVector3D(-.5, -.5, -.5), QVector3D(+.5, -.5, -.5), QVector3D(+.5, -.5, +.5), QVector3D(-.5, -.5, +.5) },
	{ QVector3D(-.5, +.5, +.5), QVector3D(+.5, +.5, +.5), QVector3D(+.5, +.5, -.5), QVector3D(-.5, +.5, -.5) }
};

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, m_wheelDelta + 1);	//zoom
	glTranslatef(m_dragTranslation->x(), m_dragTranslation->y(), 0);	//mouse dragging

	auto dragRotation = QMatrix4x4();
	dragRotation.rotate(m_dragRotation);
	glMultMatrixf(dragRotation.constData());	//mouse rotation


	for (auto i = 0; i < 6; i++)
	{
		glLoadName(1);
		glBegin(GL_QUADS);
		glColor4f(faceColors[i].red(), faceColors[i].green(), faceColors[i].blue(), faceColors[i].alpha());
		for (auto j = 0; j < 4; j++)
		{
			glVertex3f(cube[i][j].x(), cube[i][j].y(), cube[i][j].z());
		}
		glEnd();
	}
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	GLdouble aspect = width / (height ? height : 1);
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
		m_dragTranslation = new QVector2D(dx, dy);
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
	m_wheelDelta = event->delta();
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






