
#include "openglwidget.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QFileInfo>
#include "cameramodel.h"
#include "sceneitem.h"
#include "openglhelper.h"

#define highp
#define mediump
#define lowp
# define M_PI           3.14159265358979323846

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), QOpenGLFunctions_4_4_Compatibility()
	, m_manipulationModeFlag(false)
	, m_program(nullptr)
	, m_colorPickerProgram(nullptr)
	, m_fbo(nullptr)
	, m_projMatrixLoc(0)
	, m_mvMatrixLoc(0)
	, m_normalMatrixLoc(0)
	, m_lightPosLoc(0)
	, m_ambientColor(0)
	, m_diffuseColor(0)
	, m_specularColor(0)
	, m_specularExp(0)
	, m_colorPicker_ProjMatrixLoc(0)
	, m_colorPicker_mvMatrixLoc(0)
	, m_colorPicker_objId(0)
	, m_cameraModel(nullptr)
	, m_scene(nullptr)
	, m_tessellation(0)
	, m_wheelDelta(0)
	, m_isTessellationEnabled(false)
	, m_vertexShader(nullptr)
	, m_fragmentShader(nullptr)
	, m_colorPicker_vertexShader(nullptr)
	, m_colorPicker_fragmentShader(nullptr)
	, m_pickerImage(nullptr)
	, m_primitiveFactory(nullptr)
{
	m_lastPos = new QPoint();
	m_dragTranslation = new QVector3D;
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

OpenGLWidget::OpenGLWidget(SceneModel* scene, CameraModel* cameraModel, QWidget* parent)
	: QOpenGLWidget(parent), QOpenGLFunctions_4_4_Compatibility()
	, m_manipulationModeFlag(false)
	, m_program(nullptr)
	, m_colorPickerProgram(nullptr)
	, m_fbo(nullptr)
	, m_projMatrixLoc(0)
	, m_mvMatrixLoc(0)
	, m_normalMatrixLoc(0)
	, m_lightPosLoc(0)
	, m_ambientColor(0)
	, m_diffuseColor(0)
	, m_specularColor(0)
	, m_specularExp(0)
	, m_colorPicker_ProjMatrixLoc(0)
	, m_colorPicker_mvMatrixLoc(0)
	, m_colorPicker_objId(0)
	, m_tessellation(0)
	, m_wheelDelta(0)
	, m_isTessellationEnabled(false)
	, m_vertexShader(nullptr)
	, m_fragmentShader(nullptr)
	, m_colorPicker_vertexShader(nullptr)
	, m_colorPicker_fragmentShader(nullptr)
	, m_pickerImage(nullptr)
	, m_primitiveFactory(nullptr)
{
	m_scene = scene;
	m_cameraModel = cameraModel;
	m_lastPos = new QPoint();
	m_dragTranslation = new QVector3D;
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

OpenGLWidget::OpenGLWidget(VolumeModel* volume, CameraModel* cameraModel, QWidget* parent)
	: OpenGLWidget(parent)
{
	m_cameraModel = cameraModel;
	m_lastPos = new QPoint();
	m_dragTranslation = new QVector3D;
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::TrackMode::Sphere);
}

OpenGLWidget::~OpenGLWidget()
{
	cleanup();
}

QSize OpenGLWidget::minimumSizeHint() const
{
	return QSize(400, 200);
}

QSize OpenGLWidget::sizeHint() const
{
	return QSize(800, 400);
}


void OpenGLWidget::setTesselation(int t)
{
	m_tessellation = t;
	update();
}

void OpenGLWidget::resetCamera()
{
	m_cameraModel->reset();
	if (m_cameraModel->isOrthographic()) 
	{
		updateOrthoProjection(width(), height());
	}

	m_wheelDelta = 0;
	m_dragTranslation = new QVector3D;
	m_lastPos = new QPoint();
	m_dragRotation = QQuaternion();
	m_trackBall = new TrackBall(m_trackBall->getMode());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	update();
}

void OpenGLWidget::cleanup()
{
	delete m_primitiveFactory;
	delete m_lastPos;
	delete m_dragTranslation;
	delete m_trackBall;
	delete m_vertexShader;
	delete m_fragmentShader;
	delete m_colorPicker_fragmentShader;
	delete m_colorPicker_vertexShader;

	makeCurrent();
	m_vbo.destroy();
	m_nbo.destroy();
	m_vao.destroy();
	delete m_program;
	m_program = nullptr;
	delete m_colorPickerProgram;
	m_colorPickerProgram = nullptr;
	delete m_fbo;
	doneCurrent();
}

void OpenGLWidget::selectedCameraMode()
{
	m_manipulationModeFlag = false;
}

void OpenGLWidget::selectedObjManipulationMode()
{
	m_manipulationModeFlag = true;
}

void OpenGLWidget::initializeGL()
{
	m_isTessellationEnabled = true;
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

	initializeOpenGLFunctions();
	OpenGLHelper::initializeGLFunc(context());

	glClearColor(.2, .2, .2, 1);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initializeFrameBufferObject(width(), height());
	initializeSceneShaderProgram();
	initializeColorPickerShaderProgram();

	m_camera.setToIdentity();

	m_primitiveFactory = new OpenGLPrimitiveFactory(this);
}

void OpenGLWidget::paintGL()
{
	//auto items = m_scene->getAllItems();

	manipulateScene();

	paintFocusHighlight();

	m_fbo->bind();
	m_program->bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	//paintWithSceneShaderProgram(&items);
	m_program->release();

	m_colorPickerProgram->bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//paintWithColorPickerProgram(&items);
	m_colorPickerProgram->release();
	m_fbo->release();

	/*if (m_pickerImage)
	{
		delete m_pickerImage;
	}*/
	//m_pickerImage = new QImage(m_fbo->toImage(false, 1));
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_proj.setToIdentity();

	if (m_cameraModel->isOrthographic())
	{
		updateOrthoProjection(width, height);
	}
	else
	{
		m_proj.perspective(m_fov, calculateAspectRatio(width, height), m_zNear, m_zFar);
	}
	
	delete m_fbo;
	initializeFrameBufferObject(width, height);

	update();
}


void OpenGLWidget::initializeFrameBufferObject(int width, int height)
{
	makeCurrent();
	auto drawRectSize = QRect(0, 0, width, height).size();
	QOpenGLFramebufferObjectFormat format;
	format.setSamples(16);
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	m_fbo = new QOpenGLFramebufferObject(drawRectSize, format);
	m_fbo->bind();
	m_fbo->addColorAttachment(drawRectSize, GL_TEXTURE_2D);
	GLenum bufs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, bufs);
	m_fbo->release();	
}

void OpenGLWidget::initializeSceneShaderProgram()
{
	m_program = new QOpenGLShaderProgram;

	initializeSceneShaders();

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

	m_program->release();
}

void OpenGLWidget::initializeColorPickerShaderProgram()
{
	m_colorPickerProgram = new QOpenGLShaderProgram;

	initializeColorPickerShaders();

	m_colorPickerProgram->bindAttributeLocation("in_position", 0);
	if (!m_colorPickerProgram->link())
	{
		qWarning() << "Linking Error" << m_colorPickerProgram->log();
	}

	m_colorPickerProgram->bind();
	m_colorPicker_ProjMatrixLoc = m_colorPickerProgram->uniformLocation("projMatrix");
	m_colorPicker_mvMatrixLoc = m_colorPickerProgram->uniformLocation("mvMatrix");
	m_colorPicker_objId = m_colorPickerProgram->uniformLocation("objId");

	m_colorPickerProgram->release();
}

void OpenGLWidget::initializeSceneShaders()
{
	QFileInfo vsh(m_vshFile);
	if (vsh.exists())
	{
		m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
		if (m_vertexShader->compileSourceFile(m_vshFile))
		{
			m_program->addShader(m_vertexShader);
		}
		else
		{
			qWarning() << "Vertex Shader Error" << m_vertexShader->log();
		}
	}
	else
	{
		qWarning() << "Vertex Shader source file" << m_vshFile << " not found.";
	}

	QFileInfo fsh(m_fshFile);
	if (fsh.exists())
	{
		m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
		if (m_fragmentShader->compileSourceFile(m_fshFile))
		{
			m_program->addShader(m_fragmentShader);
		}
		else
		{
			qWarning() << "Fragment Shader Error" << m_fragmentShader->log();
		}
	}
	else
	{
		qWarning() << "Fragment Shader source file" << m_fshFile << " not found.";
	}
}

void OpenGLWidget::initializeColorPickerShaders()
{
	QFileInfo vsh(m_colorPicker_vshFile);
	if (vsh.exists())
	{
		m_colorPicker_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
		if (m_colorPicker_vertexShader->compileSourceFile(m_colorPicker_vshFile))
		{
			m_colorPickerProgram->addShader(m_colorPicker_vertexShader);
		}
		else
		{
			qWarning() << "Vertex Shader Error" << m_colorPicker_vertexShader->log();
		}
	}
	else
	{
		qWarning() << "Vertex Shader source file" << m_colorPicker_vshFile << " not found.";
	}

	QFileInfo fsh(m_colorPicker_fshFile);
	if (fsh.exists())
	{
		m_colorPicker_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
		if (m_colorPicker_fragmentShader->compileSourceFile(m_colorPicker_fshFile))
		{
			m_colorPickerProgram->addShader(m_colorPicker_fragmentShader);
		}
		else
		{
			qWarning() << "Fragment Shader Error" << m_colorPicker_fragmentShader->log();
		}
	}
	else
	{
		qWarning() << "Fragment Shader source file" << m_colorPicker_fshFile << " not found.";
	}
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
		m_dragTranslation->setX(dx * m_damping);
		m_dragTranslation->setY(dy * m_damping);
	}
	else
	{
		m_trackBall->release(pixelPosToViewPos(event->pos()), m_trackBall->rotation().conjugate());
	}

	update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (!hasFocus()) 
	{
		setFocus(Qt::MouseFocusReason);
	}

	m_lastPos = new QPoint(event->pos());

	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->push(pixelPosToViewPos(event->pos()));
	}

	//processSelection(); //todo fix processing

	update();
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

void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Delete)
	{
		m_scene->deleteSelectedItem();
	}
	else if (e->key() == Qt::Key_Control)
	{
		m_manipulationModeFlag = !m_manipulationModeFlag;
	}
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Control)
	{
		m_manipulationModeFlag = !m_manipulationModeFlag;
	}
	update();
}

void OpenGLWidget::focusInEvent(QFocusEvent* event)
{
	update();
}

void OpenGLWidget::focusOutEvent(QFocusEvent* event)
{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	
	m_program->setUniformValue(m_projMatrixLoc, m_proj);
	m_program->setUniformValue(m_lightPosLoc, QVector3D(m_lightPos[0], m_lightPos[1], m_lightPos[2]));
	/*m_program->setUniformValue(m_diffuseColor, QVector3D(m_kd[0], m_kd[1], m_kd[2]));
	m_program->setUniformValue(m_specularColor, QVector3D(m_ks[0], m_ks[1], m_ks[2]));
	m_program->setUniformValue(m_specularExp, m_specExp);*/
	m_program->setUniformValue(m_ambientColor, QVector3D(0.7, 0.7, 0.7));
	m_program->setUniformValue(m_diffuseColor, QVector3D(.7, 0.0, 0.0));
	//m_program->setUniformValue(m_ambientColor, QVector3D(faceColors[0][0], faceColors[0][1], faceColors[0][2]));
	
	for (auto i = 0; i < items->count(); i++)
	{
		auto item = items->at(i);
		m_world = item->getRigidBodyTransformation()->getWorldMatrix();

		m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
		auto normalMatrix = m_world.normalMatrix();
		m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

		m_primitiveFactory->createPrimitive(item->getPrimitive())->draw(m_program);
	}
}

void OpenGLWidget::paintWithColorPickerProgram(QList<SceneItem*> *items)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_colorPickerProgram->setUniformValue(m_colorPicker_ProjMatrixLoc, m_proj);
	
	for (auto i = 0; i < items->count(); i++)
	{
		auto item = items->at(i);
		m_world = item->getRigidBodyTransformation()->getWorldMatrix();

		m_colorPickerProgram->setUniformValue(m_colorPicker_mvMatrixLoc, m_camera * m_world);
		auto color = item->getId().getIdAsColor();
		m_colorPickerProgram->setUniformValue(m_colorPicker_objId, color);

		m_primitiveFactory->createPrimitive(item->getPrimitive())->draw(m_colorPickerProgram);
	}
}

void OpenGLWidget::paintFocusHighlight()
{
	if (hasFocus())
	{
		glClearColor(.3, .3, .5, 1.0);

		/*QPainter painter;
		painter.begin(this);
		painter.setRenderHint(QPainter::Antialiasing);


		painter.setPen(QPen(Qt::red, 12, Qt::SolidLine, Qt::RoundCap));
		painter.drawLine(0, 0, 0, 200);
		painter.drawLine(0, 200, 200, 200);
		painter.drawLine(200, 200, 200, 0);
		painter.drawLine(200, 0, 0, 0);

		painter.end();*/
	}
	else
	{
		glClearColor(.2, .2, .2, 1);
	}
}

void OpenGLWidget::manipulateScene()
{
	if (m_manipulationModeFlag)	//todo move this code to button event handlers to support ctrl swapping
	{
		auto selectedItem = m_scene->getSelectedItem();
		auto rigidBodyTransform = selectedItem->getRigidBodyTransformation();
		rigidBodyTransform->move(*m_dragTranslation);
		rigidBodyTransform->rotate(m_dragRotation);
	}
	else
	{
		m_cameraModel->move(m_dragTranslation);
		if (!m_cameraModel->isOrthographic())
		{
			m_cameraModel->rotate(m_dragRotation);
		}
	}

	m_cameraModel->zoom(m_wheelDelta);
	m_camera = m_cameraModel->getCameraMatrix();
}


void OpenGLWidget::updateOrthoProjection(int width, int height)
{
	float top, bottom, left, right;
	float aspect = calculateAspectRatio(width, height);

	if (width > height)
	{
		top = 1.0f;
		bottom = -top;
		right = top * aspect;
		left = -right;
	}
	else
	{
		right = 1.0f;
		left = -right;
		top = right / aspect;
		bottom = -top;
	}
	m_proj.ortho(left, right, bottom, top, m_zNear, m_zFar);
}

GLfloat OpenGLWidget::calculateAspectRatio(int width, int height)
{
	return GLfloat(width) / (height ? height : 1);
}

void OpenGLWidget::processSelection() const
{
	auto color = m_pickerImage->pixelColor(*m_lastPos);
	QVector3D colorVec(float(color.red()) / 255.0f, float(color.green()) / 255.0f, float(color.blue()) / 255.0f);
	auto id = ObjectID::getIdFromColor(colorVec);
	m_scene->updateSelectedItem(id);
}




