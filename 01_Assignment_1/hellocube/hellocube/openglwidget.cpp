
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
{
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	//todo
	update();
}


void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	const GLfloat lightPos[4] = { 0.5f, 0.0f, 0.2f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

static const GLfloat cube[] = {
	-.5f,-.5f,.5f,
	-.5f,-.5f,-.5f,
	.5f,-.5f,.5f,
	.5f,-.5f,-.5f,
	-.5f,.5f,.5f,
	-.5f,.5f,-.5f,
	.5f,.5f,.5f,
	.5f,.5f,-.5f,
};

void OpenGLWidget::paintGL()
{
	//glTranslatef(0, 0, m_wheelDelta);	//zoom
	//glTranslatef(m_dragTranslation->x(), m_dragTranslation->y(), 0);	//mouse dragging

	//auto dragRotation = QMatrix4x4();
	//dragRotation.rotate(*m_dragRotation);
	//glMultMatrixf(dragRotation.constData());	//mouse rotation

	////todo
	glBegin(GL_QUADS);

	glVertex3f(cube[1], cube[2], cube[3]);
	glVertex3f(cube[4], cube[5], cube[6]);
	glVertex3f(cube[7], cube[8], cube[9]);
	glVertex3f(cube[10], cube[11], cube[12]);

	glVertex3f(cube[13], cube[14], cube[15]);
	glVertex3f(cube[16], cube[17], cube[18]);
	glVertex3f(cube[19], cube[20], cube[21]);
	glVertex3f(cube[22], cube[23], cube[24]);

	glEnd();
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLdouble aspect = width / (height ? height : 1);

	const GLdouble zNear = 0.01f;
	const GLdouble zFar = 100.0f;
	const GLdouble fov = 45.0f;

	perspective(fov, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);

	update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	auto dx = event->x() - m_lastPos->x();
	auto dy = event->y() - m_lastPos->y();

	if (event->buttons() & Qt::LeftButton)
	{
		//todo rotation
		m_dragRotation = new QQuaternion(0,0,0,0);
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_dragTranslation = new QVector2D(dx, dy);
	}

	update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	m_lastPos = new QPoint(event->pos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		//m_trackBall->release(); //todo
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






