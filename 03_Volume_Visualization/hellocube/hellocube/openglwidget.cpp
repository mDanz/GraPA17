
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
	//, m_manipulationModeFlag(false)
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
	//, m_tessellation(0)
	//, m_wheelDelta(0)
	//, m_isTessellationEnabled(false)
	//, m_vertexShader(nullptr)
	//, m_fragmentShader(nullptr)
	//, m_colorPicker_vertexShader(nullptr)
	//, m_colorPicker_fragmentShader(nullptr)
	//, m_pickerImage(nullptr)
	, m_primitiveFactory(nullptr)
{
	//m_lastPos = new QPoint();
	//m_dragTranslation = new QVector3D;
	//m_dragRotation = QQuaternion();
	//m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

//OpenGLWidget::OpenGLWidget(SceneModel* scene, CameraModel* cameraModel, QWidget* parent)
//	: QOpenGLWidget(parent)
//	, m_manipulationModeFlag(false)
//	, m_program(nullptr)
//	, m_colorPickerProgram(nullptr)
//	, m_fbo(nullptr)
//	, m_projMatrixLoc(0)
//	, m_mvMatrixLoc(0)
//	, m_normalMatrixLoc(0)
//	, m_lightPosLoc(0)
//	, m_ambientColor(0)
//	, m_diffuseColor(0)
//	, m_specularColor(0)
//	, m_specularExp(0)
//	, m_colorPicker_ProjMatrixLoc(0)
//	, m_colorPicker_mvMatrixLoc(0)
//	, m_colorPicker_objId(0)
//	, m_tessellation(0)
//	, m_wheelDelta(0)
//	, m_isTessellationEnabled(false)
//	, m_vertexShader(nullptr)
//	, m_fragmentShader(nullptr)
//	, m_colorPicker_vertexShader(nullptr)
//	, m_colorPicker_fragmentShader(nullptr)
//	, m_pickerImage(nullptr)
//	, m_primitiveFactory(nullptr)
//{
//	m_scene = scene;
//	m_cameraModel = cameraModel;
//	m_lastPos = new QPoint();
//	m_dragTranslation = new QVector3D;
//	m_dragRotation = QQuaternion();
//	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
//}

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

//
//void OpenGLWidget::setTesselation(int t)
//{
//	m_tessellation = t;
//	update();
//}

//void OpenGLWidget::resetCamera()
//{
//	m_cameraModel->reset();
//	if (m_cameraModel->isOrthographic()) 
//	{
//		updateOrthoProjection(width(), height());
//	}
//
//	m_wheelDelta = 0;
//	m_dragTranslation = new QVector3D;
//	m_lastPos = new QPoint();
//	m_dragRotation = QQuaternion();
//	m_trackBall = new TrackBall(m_trackBall->getMode());
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	update();
//}

void OpenGLWidget::cleanup()
{
	delete m_primitiveFactory;
	//delete m_lastPos;
	//delete m_dragTranslation;
	//delete m_trackBall;
	//delete m_vertexShader;
	//delete m_fragmentShader;
	//delete m_colorPicker_fragmentShader;
	//delete m_colorPicker_vertexShader;

	makeCurrent();
	delete m_program;
	m_program = nullptr;
	delete m_highlightProgram;
	m_highlightProgram = nullptr;
	delete m_fbo;
	doneCurrent();
}

//void OpenGLWidget::selectedCameraMode()
//{
//	m_manipulationModeFlag = false;
//}
//
//void OpenGLWidget::selectedObjManipulationMode()
//{
//	m_manipulationModeFlag = true;
//}

void OpenGLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

	initializeOpenGLFunctions();
	OpenGLHelper::initializeGLFunc(context());

	initializeFrameBufferObject(width(), height());
	initializeSceneShaderProgram();
	initializeHighlightShaderProgram();

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// enable culling
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

	//manipulateScene();

	m_fbo->bind();
	paintWithSceneShaderProgram(&items);
	paintWithHighlightShaderProgram(&items);
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
	//auto drawRectSize = QRect(0, 0, width, height).size();
	//QOpenGLFramebufferObjectFormat format;
	//format.setSamples(16);
	//format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	m_fbo = new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::CombinedDepthStencil);
	//m_fbo = new QOpenGLFramebufferObject(drawRectSize, format);
	m_fbo->bind();
	m_fbo->addColorAttachment(width, height);
	//m_fbo->addColorAttachment(drawRectSize, GL_TEXTURE_2D);
	//GLenum bufs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	//glDrawBuffers(2, bufs);
	//m_fbo->release();	
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

	/*auto dx = event->x() - m_lastPos->x();
	auto dy = event->y() - m_lastPos->y();

	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->move(pixelPosToScreenPos(event->pos()), m_trackBall->getRotation().conjugate());
		m_dragRotation = m_trackBall->getRotation();
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_trackBall->release(pixelPosToScreenPos(event->pos()), m_trackBall->getRotation().conjugate());
		m_dragTranslation->setX(dx * m_damping);
		m_dragTranslation->setY(dy * m_damping);
	}
	else
	{
		m_trackBall->release(pixelPosToScreenPos(event->pos()), m_trackBall->getRotation().conjugate());
	}*/

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

void OpenGLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble left, right, bottom, top;

	top = zNear * tan(fovy * M_PI / 360.0f);
	bottom = -top;
	left = bottom * aspect;
	right = top * aspect;

	glFrustum(left, right, bottom, top, zNear, zFar);
}

void OpenGLWidget::paintWithSceneShaderProgram(QList<SceneItem*> *items)
{
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OpenGLHelper::getGLFunc()->glDrawBuffers(2, buffers);
	m_program->bind();
	m_program->setUniformValue(m_projMatrixLoc, *m_cameraModel->getProjectionMatrix());//todo use cameramodel projectionmatrix
	m_program->setUniformValue(m_lightPosLoc, QVector3D(m_lightPos[0], m_lightPos[1], m_lightPos[2]));
	m_program->setUniformValue(m_ambientColor, QVector3D(m_ka[0], m_ka[1], m_ka[2])); //todo use variable
	m_program->setUniformValue(m_diffuseColor, QVector3D(m_kd[0], m_kd[1], m_kd[2]));
	m_program->setUniformValue(m_specularColor, QVector3D(m_ks[0], m_ks[1], m_ks[2]));
	m_program->setUniformValue(m_specularExp, m_specExp);
	//m_program->setUniformValue(m_ambientColor, QVector3D(faceColors[0][0], faceColors[0][1], faceColors[0][2]));

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

//void OpenGLWidget::paintFocusHighlight()
//{
//	if (hasFocus())
//	{
//		glClearColor(.3, .3, .5, 1.0);
//	}
//	else
//	{
//		glClearColor(.2, .2, .2, 1);
//	}
//}

//void OpenGLWidget::manipulateScene()
//{
//	if (m_manipulationModeFlag)	//todo move this code to button event handlers to support ctrl swapping
//	{
//		auto selectedItem = m_scene->getSelectedItem();
//		auto rigidBodyTransform = selectedItem->getRigidBodyTransformation();
//		rigidBodyTransform->move(*m_dragTranslation);
//		rigidBodyTransform->rotate(m_dragRotation);
//	}
//	else
//	{
//		m_cameraModel->move(m_dragTranslation);
//		if (!m_cameraModel->isOrthographic())
//		{
//			m_cameraModel->rotate(m_dragRotation);
//		}
//	}
//
//	m_cameraModel->zoom(m_wheelDelta);
//	m_camera = m_cameraModel->getCameraMatrix();
//}

//
//void OpenGLWidget::updateOrthoProjection(int width, int height)
//{
//	float top, bottom, left, right;
//	float aspect = calculateAspectRatio(width, height);
//
//	if (width > height)
//	{
//		top = 1.0f;
//		bottom = -top;
//		right = top * aspect;
//		left = -right;
//	}
//	else
//	{
//		right = 1.0f;
//		left = -right;
//		top = right / aspect;
//		bottom = -top;
//	}
//	m_proj.ortho(left, right, bottom, top, m_zNear, m_zFar);
//}

//void OpenGLWidget::processSelection() const
//{
//	auto color = m_pickerImage->pixelColor(*m_lastPos);
//	QVector3D colorVec(float(color.red()) / 255.0f, float(color.green()) / 255.0f, float(color.blue()) / 255.0f);
//	auto id = ObjectID::getIdFromColor(colorVec);
//	auto item = m_scene->getItem(id);
//	m_scene->selectItem(item);
//}

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



