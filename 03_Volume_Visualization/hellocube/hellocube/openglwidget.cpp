
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
#include "volumemodel.h"

# define M_PI           3.14159265358979323846

static const float boxVx[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};
static const float quadVx[] = { -1,-1,0, 1,1,0, -1,1,0,  -1,-1,0, 1,-1,0, 1,1,0, };

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, m_isSelected(false)
	, m_displaymode(4)
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
	delete m_entryExitFbo;
	doneCurrent();
}


void OpenGLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

	initializeOpenGLFunctions();
	OpenGLHelper::initializeGLFunc(context());

	initializeFrameBufferObjects(width(), height());
	initializeSceneShaderProgram();
	initializeHighlightShaderProgram();
	initializeEntryExitShaderProgram();
	initializeVolumeShaderProgram();

	//glEnable(GL_BLEND); //todo do I need it really
	//glAlphaFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, new float[3]{ m_lightPos.x(), m_lightPos.y(), m_lightPos.z() });
	glEnable(GL_LIGHT0);

	m_primitiveFactory = new OpenGLPrimitiveFactory();
}

void OpenGLWidget::paintGL()
{
	auto items = m_scene->getAllItems();

	m_fbo->bind();
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_fbo->release();


	paintWithSceneShaderProgram(&items);
	paintWithVolumeShaderProgram(&items);
	paintWithHighlightShaderProgram();
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_cameraModel->resizeViewPort(width, height);
	
	delete m_fbo;
	delete m_entryExitFbo;
	initializeFrameBufferObjects(width, height);

	update();
}


void OpenGLWidget::initializeFrameBufferObjects(int width, int height)
{
	auto drawRectSize = QRect(0, 0, width, height).size();
	QOpenGLFramebufferObjectFormat format;
	format.setTextureTarget(GL_TEXTURE_2D);
	format.setInternalTextureFormat(GL_RGBA8);
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	m_fbo = new QOpenGLFramebufferObject(drawRectSize, format);
	m_fbo->bind();
	m_fbo->addColorAttachment(drawRectSize, format.internalTextureFormat());
	m_fbo->release();
	qInfo() << "m_fbo " << (m_fbo->isValid() ? "is valid " : "IS NOT VALID! ") << "size: " << m_fbo->width() << "," << m_fbo->height();

	m_entryExitFbo = new QOpenGLFramebufferObject(drawRectSize, format);
	m_entryExitFbo->bind();
	m_entryExitFbo->addColorAttachment(drawRectSize, format.internalTextureFormat());
	m_entryExitFbo->release();
	qInfo() << "m_entryExitFbo " << (m_entryExitFbo->isValid() ? "is valid " : "IS NOT VALID! ") << "size: " << m_entryExitFbo->width() << "," << m_entryExitFbo->height();
	
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

void OpenGLWidget::initializeEntryExitShaderProgram()
{
	m_entryExitProgram = OpenGLHelper::createShaderProgam(m_entryExit_vshFile, m_entryExit_fshFile);
	m_program->bindAttributeLocation("in_position", 0);
	if (!m_entryExitProgram->link())
	{
		qWarning() << "Linking Error" << m_entryExitProgram->log();
	}

	m_entryExitProgram->bind();
	m_mvMatrixLoc = m_entryExitProgram->uniformLocation("mvMatrix");
	m_projMatrixLoc = m_entryExitProgram->uniformLocation("projMatrix");
}

void OpenGLWidget::initializeVolumeShaderProgram()
{
	m_volumeShaderProgram = OpenGLHelper::createShaderProgam(m_volume_vshFile, m_volume_fshFile);
	m_program->bindAttributeLocation("in_position", 0);
	if (!m_program->link())
	{
		qWarning() << "Linking Error" << m_volumeShaderProgram->log();
	}
	m_volumeShaderProgram->bind();
	m_volumeShaderProgram->setUniformValue("volumeSampler", 0);
	m_volumeShaderProgram->setUniformValue("entryPoints", 1);
	m_volumeShaderProgram->setUniformValue("exitPoints", 2);
	m_volumeShaderProgram->setUniformValue("transferFunction", 3);
	m_volumeShaderProgram->release();

	OpenGLHelper::getGLFunc()->glGenBuffers(1, &m_quadVbo);
	OpenGLHelper::getGLFunc()->glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
	OpenGLHelper::getGLFunc()->glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), quadVx, GL_STATIC_DRAW);

	OpenGLHelper::getGLFunc()->glGenBuffers(1, &m_boxVbo);
	OpenGLHelper::getGLFunc()->glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo);
	OpenGLHelper::getGLFunc()->glBufferData(GL_ARRAY_BUFFER, 6 * 2 * 3 * 3 * sizeof(float), boxVx, GL_STATIC_DRAW);
}

QPointF OpenGLWidget::pixelPosToScreenPos(const QPointF &pos) const
{
	//todo fix needed
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

void OpenGLWidget::paintWithSceneShaderProgram(QList<SceneItem*> *items)
{
	m_fbo->bind();

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OpenGLHelper::getGLFunc()->glDrawBuffers(2, buffers);

	m_program->bind();
	m_program->setUniformValue("projMatrix", *m_cameraModel->getProjectionMatrix());
	m_program->setUniformValue("lightPos", m_lightPos);
	m_program->setUniformValue("ambientColor", m_ka); //todo use variable
	m_program->setUniformValue("diffuseColor", m_kd);
	m_program->setUniformValue("specularColor", m_ks);
	m_program->setUniformValue("specularExp", m_specExp);

	for (auto i = 0; i < items->count(); i++)
	{
		auto item = items->at(i);
		if (item->getPrimitiveType() == Volume)
		{
			continue;
		}
		m_world = item->getRigidBodyTransformation()->getWorldMatrix();

		m_program->setUniformValue("idColor", item->getId()->getIdAsColor());
		m_program->setUniformValue("mvMatrix", *m_cameraModel->getCameraMatrix() * m_world);
		m_program->setUniformValue("normalMatrix", m_world.normalMatrix());

		m_primitiveFactory->renderPrimitive(item->getPrimitiveType())->draw(m_program);
	}
	glFlush();

	m_program->release();
	m_fbo->release();

	auto img1 = m_fbo->toImage(false, 0);
	if (!img1.save("./tex/sceneTex.jpg"))
	{
		qWarning() << "Scene Texture not saved correctly";
	}auto img2 = m_fbo->toImage(false, 1);
	if (!img2.save("./tex/pickTex.jpg"))
	{
		qWarning() << "Pick Texture not saved correctly";
	}

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "phong shader program errors:" << err;
	}


}

void OpenGLWidget::paintWithHighlightShaderProgram() //todo refactor this
{
	auto glFunc = OpenGLHelper::getGLFunc();

	GLenum defBuf[] = { GL_COLOR_ATTACHMENT0 };
	glFunc->glDrawBuffers(1, defBuf);
	qInfo() << "highlight shader debug:" << OpenGLHelper::Error();

	m_highlightProgram->bind();

	glEnable(GL_TEXTURE_2D);
	glFunc->glActiveTexture(GL_TEXTURE0);
	glFunc->glBindTexture(GL_TEXTURE_2D, m_fbo->textures()[0]);
	glFunc->glActiveTexture(GL_TEXTURE1);
	glFunc->glBindTexture(GL_TEXTURE_2D, m_fbo->textures()[1]);

	m_highlightProgram->setUniformValue("isSelectedWidget", m_isSelected);
	m_highlightProgram->setUniformValue("highlightColor", m_highlightColor);
	if (auto selectedItem = m_scene->getSelectedItem()) 
	{
		m_highlightProgram->setUniformValue("selectedID", QVector4D(selectedItem->getId()->getIdAsColor(), 1.0));
	}
	else 
	{
		m_highlightProgram->setUniformValue("selectedID", QVector4D(0.0, 1.0, 0.0, 0.0));
	}

	glColor4f(0, 0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();

	glFlush();

	m_highlightProgram->release();

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "highlight shader program errors:" << err;
	}
}

void OpenGLWidget::paintWithVolumeShaderProgram(QList<SceneItem*> *items) 
{
	QString err = OpenGLHelper::Error();

	for(int i = 0; i < items->size(); i++)
	{
		auto item = items->at(i);
		if (item->getPrimitiveType() != Volume)
		{
			continue;
		}
		auto volume = reinterpret_cast<VolumeModel*>(item);

		renderEntryExitPoints(volume);
		renderVolumeData(volume);

	}	
}

void OpenGLWidget::renderEntryExitPoints(VolumeModel *volume)
{
	m_entryExitFbo->bind();
	m_entryExitProgram->bind();

	renderExitPoints(volume);
	renderEntryPoints();

	m_entryExitProgram->release();
	m_entryExitFbo->release();

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "entry/exit errors:" << err;
	}
}

void OpenGLWidget::renderExitPoints(VolumeModel* volume)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	OpenGLHelper::getGLFunc()->glDrawBuffer(GL_COLOR_ATTACHMENT0);
	m_entryExitProgram->setUniformValue("mvMatrix", *(m_cameraModel->getCameraMatrix()) * volume->getLocalMatrix());
	m_entryExitProgram->setUniformValue("projMatrix", *(m_cameraModel->getProjectionMatrix()));

	OpenGLHelper::getGLFunc()->glEnableVertexAttribArray(0);
	OpenGLHelper::getGLFunc()->glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo);
	OpenGLHelper::getGLFunc()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid*>(nullptr));
	OpenGLHelper::getGLFunc()->glColor3f(1.0, 0.0, 0.0);
	OpenGLHelper::getGLFunc()->glDrawArrays(GL_TRIANGLES, 0, 36);

	glFlush();

	auto img1 = m_entryExitFbo->toImage(false, 0);
	if (!img1.save("./tex/exitTex.jpg"))
	{
		qWarning() << "Exit Texture not saved correctly";
	}

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "exit point errors:" << err;
	}
}

void OpenGLWidget::renderEntryPoints()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	OpenGLHelper::getGLFunc()->glDrawBuffer(GL_COLOR_ATTACHMENT1);

	OpenGLHelper::getGLFunc()->glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo);
	OpenGLHelper::getGLFunc()->glDrawArrays(GL_TRIANGLES, 0, 36);
	OpenGLHelper::getGLFunc()->glDisableVertexAttribArray(0);

	glFlush();

	auto img2 = m_entryExitFbo->toImage(false, 1);
	if (!img2.save("./tex/entryTex.jpg"))
	{
		qWarning() << "Entry Texture not saved correctly";
	}

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "entry point errors:" << err;
	}
}

void OpenGLWidget::renderVolumeData(VolumeModel *volume)
{
	
	m_volumeShaderProgram->bind();
	glCullFace(GL_BACK);

	m_fbo->bind();
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OpenGLHelper::getGLFunc()->glDrawBuffers(2, buffers);

	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	m_volumeShaderProgram->setUniformValue("displayMode", m_scene->getDisplayMode());

	m_volumeShaderProgram->setUniformValue("width", static_cast<int>(volume->getDimensions()->x()));
	m_volumeShaderProgram->setUniformValue("height", static_cast<int>(volume->getDimensions()->y()));
	m_volumeShaderProgram->setUniformValue("depth", static_cast<int>(volume->getDimensions()->z()));
	m_volumeShaderProgram->setUniformValue("minValue", volume->getMinValue());
	m_volumeShaderProgram->setUniformValue("maxValue", volume->getMaxValue());

	m_volumeShaderProgram->setUniformValue("step", 0.01f);
	m_volumeShaderProgram->setUniformValue("viewMatrix", *m_cameraModel->getCameraMatrix());
	//m_volumeShaderProgram->setUniformValue("mvMatrix", *m_cameraModel->getCameraMatrix());
	//m_volumeShaderProgram->setUniformValue("projMatrix", *m_cameraModel->getProjectionMatrix());
	m_volumeShaderProgram->setUniformValue("normalMatrix", m_cameraModel->getCameraMatrix()->normalMatrix());
	m_volumeShaderProgram->setUniformValue("idColor", QVector4D(volume->getId()->getIdAsColor(), 1.0));

	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	OpenGLHelper::getGLFunc()->glActiveTexture(GL_TEXTURE0);
	OpenGLHelper::getGLFunc()->glBindTexture(GL_TEXTURE_3D, volume->getTextureName());
	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	OpenGLHelper::getGLFunc()->glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_entryExitFbo->textures()[1]);
	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	OpenGLHelper::getGLFunc()->glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_entryExitFbo->textures()[0]);
	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	OpenGLHelper::getGLFunc()->glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, volume->getTransferFunction()->getTextureName());
	qInfo() << "volume shader debug:" << OpenGLHelper::Error();

	OpenGLHelper::getGLFunc()->glEnableVertexAttribArray(0);
	OpenGLHelper::getGLFunc()->glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
	OpenGLHelper::getGLFunc()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid*>(nullptr));
	OpenGLHelper::getGLFunc()->glDrawArrays(GL_TRIANGLES, 0, 6);

	OpenGLHelper::getGLFunc()->glDisableVertexAttribArray(0);

	glFlush();

	m_volumeShaderProgram->release();
	m_fbo->release();

	auto img1 = m_fbo->toImage(false, 0);
	if (!img1.save("./tex/volumeTex.jpg"))
	{
		qWarning() << "Volume Texture not saved correctly";
	}
	auto img2 = m_fbo->toImage(false, 1);
	if (!img2.save("./tex/volumePickTex.jpg"))
	{
		qWarning() << "Volume Pick Texture not saved correctly";
	}

	auto err = OpenGLHelper::Error();
	if (!err.isEmpty())
	{
		qInfo() << "volume shader program errors:" << err;
	}
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



