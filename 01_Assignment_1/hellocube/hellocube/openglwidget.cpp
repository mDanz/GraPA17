
#include "openglwidget.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

#define highp
#define mediump
#define lowp
# define M_PI           3.14159265358979323846

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), QOpenGLFunctions_4_4_Compatibility()
	, m_tessellation(0)
	, m_wheelDelta(0)
{
	m_lastPos = new QPoint();
	m_dragTranslation = new GLfloat[2]{0, 0};
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

OpenGLWidget::~OpenGLWidget()
{
	cleanup();
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
	m_isTessellationEnabled = true;

	makeCurrent();
	m_program->release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	update();
}

void OpenGLWidget::flatShading()
{
	m_isTessellationEnabled = true;

	makeCurrent();
	m_program->release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	update();
}

void OpenGLWidget::gouraudShading()
{
	m_isTessellationEnabled = true;

	makeCurrent();
	m_program->release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	update();
}

void OpenGLWidget::phongShading()
{
	m_isTessellationEnabled = false;
	makeCurrent();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_program->bind();
	update();
}

void OpenGLWidget::setTesselation(int t)
{
	m_tessellation = t;
	update();
}

void OpenGLWidget::resetCamera()
{
	//m_tessellation = 0;
	m_wheelDelta = 0;
	m_dragTranslation = new GLfloat[2]{0, 0};
	m_lastPos = new QPoint();
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(m_trackBall->getMode());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	update();
}

void OpenGLWidget::cleanup()
{
	delete m_lastPos;
	delete [] m_dragTranslation;
	delete m_trackBall;

	makeCurrent();
	m_vbo.destroy();
	delete m_program;
	m_program = nullptr;
	doneCurrent();
}

static const char *vertexShaderSource =
"attribute vec4 vertex;\n"
"attribute vec3 normal;\n"
"varying vec3 vert;\n"
"varying vec3 vertNormal;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   vertNormal = normalMatrix * normal;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"}\n";

static const char *fragmentShaderSource =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"uniform highp vec3 lightPos;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
"   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
"   gl_FragColor = vec4(col, 1.0);\n"
"}\n";


void OpenGLWidget::initializeGL()
{
	m_isTessellationEnabled = true;
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

	initializeOpenGLFunctions();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	initializeShaderProgram();
}

//static const float cubeVertices[6][4][3] = {
//	{ {+.5, -.5, +.5}, {+.5, -.5, -.5}, {+.5, +.5, -.5}, {+.5, +.5, +.5} },
//	{ {-.5, -.5, -.5}, {-.5, -.5, +.5}, {-.5, +.5, +.5}, {-.5, +.5, -.5} },
//	{ {+.5, -.5, -.5}, {-.5, -.5, -.5}, {-.5, +.5, -.5}, {+.5, +.5, -.5} },
//	{ {-.5, -.5, +.5}, {+.5, -.5, +.5}, {+.5, +.5, +.5}, {-.5, +.5, +.5} },
//	{ {-.5, -.5, -.5}, {+.5, -.5, -.5}, {+.5, -.5, +.5}, {-.5, -.5, +.5} },
//	{ {-.5, +.5, +.5}, {+.5, +.5, +.5}, {+.5, +.5, -.5}, {-.5, +.5, -.5} }
//};


static const float cubeVertices[8*3] = {
	+.5, -.5, +.5,
	+.5, -.5, -.5,
	+.5, +.5, -.5,
	+.5, +.5, +.5,
	-.5, -.5, -.5,
	-.5, -.5, +.5,
	-.5, +.5, +.5,
	-.5, +.5, -.5
};

static const int cubeIndices[6*4] = {
	2, 3, 0, 1,
	4, 5, 6, 7,	
	1, 4, 7, 2,	
	3, 6, 5, 0,
	1, 0, 5, 4,
	7, 6, 3, 2
};

static const float normals[6*3] = {
	1, 0, 0,
	-1, 0, 0,
	0, 0, -1,
	0, 0, 1,
	0, -1, 0,
	0, 1, 0
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
	glEnable(GL_NORMALIZE);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(m_dragTranslation[0], -m_dragTranslation[1], m_wheelDelta - 5);	//mouse dragging + zoom

	auto dragRotation = QMatrix4x4();
	dragRotation.rotate(m_dragRotation);
	glMultMatrixf(dragRotation.constData());	//mouse rotation



	m_world.setToIdentity();
	m_world.translate(m_dragTranslation[0], -m_dragTranslation[1], m_wheelDelta - 5);
	m_world.rotate(m_dragRotation);

	

	if (m_isTessellationEnabled)
	{
		paintWithTessellation();
	}
	else
	{
		paintWithShaderProgram();
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

	//resize projection for shader program
	m_proj.setToIdentity();
	m_proj.perspective(m_fov, aspect, m_zNear, m_zFar);

	update();
}

void OpenGLWidget::initializeShaderProgram()
{
	m_program = new QOpenGLShaderProgram;
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_program->bindAttributeLocation("vertex", 0);
	m_program->bindAttributeLocation("normal", 1);
	m_program->link();

	m_program->bind();
	m_projMatrixLoc = m_program->uniformLocation("projMatrix");
	m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
	m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
	m_lightPosLoc = m_program->uniformLocation("lightPos");

	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(cubeVertices, sizeof(cubeVertices)/sizeof(cubeVertices[0]));

	// Store the vertex attribute bindings for the program.
	setupVertexAttribs();

	// Our camera never changes in this example.
	m_camera.setToIdentity();
	//m_camera.translate(0, 0, -5);

	// Light position is fixed.
	m_program->setUniformValue(m_lightPosLoc, QVector3D(m_lightPos[0], m_lightPos[1], m_lightPos[2]));

	m_program->release();
}

void OpenGLWidget::setupVertexAttribs()
{
	m_vbo.bind();
	auto functions = QOpenGLContext::currentContext()->functions();
	functions->glEnableVertexAttribArray(0);
	functions->glEnableVertexAttribArray(1);
	functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	functions->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
	m_vbo.release();
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

void OpenGLWidget::paintWithTessellation()
{
	float tessFactor = 1.0f / (1 + m_tessellation);
	for (auto x = 0; x <= m_tessellation; x++)
	{
		float xOffset = tessFactor * x;
		for (auto y = 0; y <= m_tessellation; y++)
		{
			float yOffset = tessFactor * y;
			for (auto z = 0; z <= m_tessellation; z++)
			{
				float zOffset = tessFactor * z;

				glTranslatef(xOffset, yOffset, zOffset);
				glScalef(tessFactor, tessFactor, tessFactor);
				
				paint();

				glScalef(1 / tessFactor, 1 / tessFactor, 1 / tessFactor);
				glTranslatef(-xOffset, -yOffset, -zOffset);
			}
		}
	}
	
}

void OpenGLWidget::paint()
{
	for (auto i = 0; i < 6; i++)
	{
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColors[i]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_specExp);
		glNormal3f(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
		for (auto j = 0; j < 4; j++)
		{
			int index = cubeIndices[i * 4 + j] * 3;
			glVertex3f(cubeVertices[index + 0], cubeVertices[index + 1], cubeVertices[index + 2]);
		}
		glEnd();
	}
}

void OpenGLWidget::paintWithShaderProgram()
{
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	//m_program->bind();
	m_program->setUniformValue(m_projMatrixLoc, m_proj);
	m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
	QMatrix3x3 normalMatrix = m_world.normalMatrix();
	m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

	//glDrawArrays(GL_TRIANGLES, 0, 8);
	paint();
	//m_program->release();
}





