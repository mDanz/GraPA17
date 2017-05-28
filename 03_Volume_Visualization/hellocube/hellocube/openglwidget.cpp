
#include "openglwidget.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QFileInfo>
#include "cameramodel.h"
#include "sceneitem.h"
#include "openglhelper.h"
#include "scenecontroller.h"
#include "scenemodel.h"

# define M_PI           3.14159265358979323846

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, m_isSelected(false)
	, m_program(nullptr)
	, m_highlightProgram(nullptr)
	, m_fbo(nullptr)
	, m_projMatrixLoc(0)
	, m_mvMatrixLoc(0)
	, m_normalMatrixLoc(0)
	, m_lightPosLoc(0)
	, m_ambientColor(0)
	, m_diffuseColor(0)
	, m_specularColor(0)
	, m_specularExp(0)
	, m_idColor(0)
	, m_colorPicker_ProjMatrixLoc(0)
	, m_colorPicker_mvMatrixLoc(0)
	, m_colorPicker_objId(0)
	, m_cameraModel(nullptr)
	, m_scene(nullptr)
	, m_primitiveFactory(nullptr)
{
}

OpenGLWidget::~OpenGLWidget()
{
	cleanup();
}

void OpenGLWidget::setModel(SceneModel* scene, CameraModel* camera)
{
	m_scene = scene;
	m_cameraModel = camera;
}

CameraModel* OpenGLWidget::getCamera() const
{
	return m_cameraModel;
}

void OpenGLWidget::select()
{
	m_isSelected = true;
}

void OpenGLWidget::deselect()
{
	m_isSelected = false;
}

QSize OpenGLWidget::minimumSizeHint() const
{
	return QSize(400, 200);
}

QSize OpenGLWidget::sizeHint() const
{
	return QSize(800, 400);
}

void OpenGLWidget::cleanup()
{
	delete m_primitiveFactory;

	makeCurrent();
	delete m_program;
	m_program = nullptr;
	delete m_highlightProgram;
	m_highlightProgram = nullptr;
	delete m_fbo;
	doneCurrent();
}


void OpenGLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

	initializeOpenGLFunctions();
	OpenGLHelper::initializeGLFunc(context());

	initializeFrameBufferObject(width(), height());
	initializeSceneShaderProgram();
	initializeHighlightShaderProgram();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glClearColor(.1, .1, .1, 1);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
	glEnable(GL_LIGHT0);

	m_primitiveFactory = new OpenGLPrimitiveFactory();
}

void OpenGLWidget::paintGL()
{
	auto items = m_scene->getAllItems();

	m_fbo->bind();
	paintWithSceneShaderProgram(&items);
	//paintWithHighlightShaderProgram(&items);
	m_fbo->release();
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_cameraModel->resizeViewPort(width, height);
	
	delete m_fbo;
	initializeFrameBufferObject(width, height);

	update();
}


void OpenGLWidget::initializeFrameBufferObject(int width, int height)
{
	makeCurrent();
	m_fbo = new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::Depth);
	m_fbo->bind();
	m_fbo->addColorAttachment(width, height);
}

void OpenGLWidget::initializeSceneShaderProgram()
{
	m_program = OpenGLHelper::createShaderProgam(m_vshFile, m_fshFile);
	m_program->bindAttributeLocation("in_position", 0);
	m_program->bindAttributeLocation("in_normal", 1);
	if (!m_program->link())
	{
		qWarning() << "Linking Error" << m_program->log();
	}

	m_program->bind();
	m_projMatrixLoc = m_program->uniformLocation("projMatrix");
	m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
	m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
	m_lightPosLoc = m_program->uniformLocation("lightPos");
	m_ambientColor = m_program->uniformLocation("ambientColor");
	m_diffuseColor = m_program->uniformLocation("diffuseColor");
	m_specularColor = m_program->uniformLocation("specularColor");
	m_specularExp  = m_program->uniformLocation("specularExp");
	m_idColor = m_program->uniformLocation("objId");
	m_program->release();
}

void OpenGLWidget::initializeHighlightShaderProgram()
{
	m_highlightProgram = OpenGLHelper::createShaderProgam(m_highlight_vshFile, m_highlight_fshFile);
	m_highlightProgram->bindAttributeLocation("in_position", 0);
	if (!m_program->link())
	{
		qWarning() << "Linking Error" << m_highlightProgram->log();
	}

	m_highlightProgram->bind();
	m_highlightProgram->setUniformValue("sceneTex", 0);
	m_highlightProgram->setUniformValue("pickTex", 1);
	m_highlightProgram->release();
}


QPointF OpenGLWidget::pixelPosToScreenPos(const QPointF &pos) const
{
	//todo maybe fix needed
	auto x = 2.0 * float(pos.x()) / width() - 1.0;
	auto y = 1.0 - 2.0 * float(pos.y()) / height();
	return QPointF(x, y);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	auto mousePos = event->pos();
	auto screenPos = pixelPosToScreenPos(mousePos);
	SceneController::getController()->mouseMoved(screenPos, mousePos, event);

	update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	auto mousePos = event->pos();
	auto screenPos = pixelPosToScreenPos(mousePos);
	SceneController::getController()->mousePressed(screenPos, mousePos, event);

	emit clickedInside();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	auto idUnderMouse = getIdFromScreen(event->pos());
	auto screenPos = pixelPosToScreenPos(event->pos());
	SceneController::getController()->mouseReleased(screenPos, idUnderMouse, event);
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	SceneController::getController()->wheelMoved(event->delta());
	update();
}

//void OpenGLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
//{
//	GLdouble left, right, bottom, top;
//
//	top = zNear * tan(fovy * M_PI / 360.0f);
//	bottom = -top;
//	left = bottom * aspect;
//	right = top * aspect;
//
//	glFrustum(left, right, bottom, top, zNear, zFar);
//}

void OpenGLWidget::paintWithSceneShaderProgram(QList<SceneItem*> *items)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OpenGLHelper::getGLFunc()->glDrawBuffers(2, buffers);
	m_program->bind();
	m_program->setUniformValue(m_projMatrixLoc, *m_cameraModel->getProjectionMatrix());
	m_program->setUniformValue(m_lightPosLoc, QVector3D(m_lightPos[0], m_lightPos[1], m_lightPos[2]));
	m_program->setUniformValue(m_ambientColor, QVector3D(m_ka[0], m_ka[1], m_ka[2])); //todo use variable
	m_program->setUniformValue(m_diffuseColor, QVector3D(m_kd[0], m_kd[1], m_kd[2]));
	m_program->setUniformValue(m_specularColor, QVector3D(m_ks[0], m_ks[1], m_ks[2]));
	m_program->setUniformValue(m_specularExp, m_specExp);

	for (auto i = 0; i < items->count(); i++)
	{
		auto item = items->at(i);
		m_world = item->getRigidBodyTransformation()->getWorldMatrix();

		m_program->setUniformValue(m_idColor, item->getId()->getIdAsColor());
		m_program->setUniformValue(m_mvMatrixLoc, *m_cameraModel->getCameraMatrix() * m_world);
		auto normalMatrix = m_world.normalMatrix();
		m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

		m_primitiveFactory->renderPrimitive(item->getPrimitiveType())->draw(m_program);
	}

	m_program->release();
	glFlush();
}

void OpenGLWidget::paintWithHighlightShaderProgram(QList<SceneItem*> *items)
{
	GLenum defBuf[] = { GL_FRONT_LEFT };
	OpenGLHelper::getGLFunc()->glDrawBuffers(1, defBuf);

	m_highlightProgram->bind();

	// bind the textures from the fbo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo->textures()[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fbo->textures()[1]);


	m_highlightProgram->setUniformValue("isSelectedWidget", m_isSelected);
	if (auto selectedItem = m_scene->getSelectedItem()) 
	{
		m_highlightProgram->setUniformValue("selectedID", selectedItem->getId()->getIdAsColor());
	}
	else 
	{
		m_highlightProgram->setUniformValue("selectedID", QColor(0, 0, 0, 0));
	}

	glColor4f(0, 0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();

	m_highlightProgram->release();
	glFlush();
}

int OpenGLWidget::getIdFromScreen(QPoint pos)
{
	m_fbo->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	float pixel[4];
	glReadPixels(pos.x(), pos.y(), 1, 1, GL_RGBA, GL_FLOAT, pixel);
	float idUnderMouse = ObjectID::getIdFromColor(QVector3D(pixel[0] / 255.0f, pixel[1] / 255.0f, pixel[2] / 255.0f));
	m_fbo->release();

	return idUnderMouse;
}



