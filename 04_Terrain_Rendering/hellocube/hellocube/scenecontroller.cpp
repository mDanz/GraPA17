
#include "scenecontroller.h"
#include "scenemodel.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include "trackball.h"
#include "volumemodelfactory.h"
#include "volumemodel.h"
#include "rigidbodytransformation.h"
#include "cameramodel.h"
#include <QWheelEvent>
#include "terrainmodelfactory.h"
#include "terrainmodel.h"

SceneController* SceneController::s_controller;

SceneController::~SceneController()
{
	delete m_trackBall;
	delete m_lastPos;
}

SceneController* SceneController::getController()
{
	return s_controller ? s_controller : s_controller = new SceneController();
}

void SceneController::setModelView(SceneModel* scene, ViewPortWidget* viewPortWidget)
{
	m_viewPortWidget = viewPortWidget;
	m_scene = scene;
}

void SceneController::setName(SceneItem* item, QString name) const
{
	item->setName(name);
	m_scene->update();
	m_scene->selectItem(item);
}

void SceneController::mousePressed(QPointF screenPos, QPointF mousePos, QMouseEvent* event)
{
	m_singleClick = true;

	delete m_lastPos;
	m_lastPos = new QPointF(screenPos);

	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->push(mousePos);
	}
}

void SceneController::mouseReleased(QPointF screenPos, int idUnderMouse, QMouseEvent* event)
{
	if (m_singleClick)
	{
		m_singleClick = false;
		m_scene->selectItem(m_scene->getItem(idUnderMouse));
	}

	if (event->button() == Qt::LeftButton)
	{
		m_trackBall->release(screenPos, m_trackBall->getRotation().conjugate());
	}
}

void SceneController::mouseMoved(QPointF screenPos, QPointF mousePos, QMouseEvent* event)
{
	m_singleClick = false;

	auto delta = new QVector3D(mousePos.x() - m_lastPos->x(), mousePos.y() - m_lastPos->y(), 0);

	if (m_isInCameraMode != (event->modifiers() & Qt::CTRL))
	{
		manipulateCamera(screenPos, delta, event);
	}
	else
	{
		manipulateSelectedItem(screenPos, delta, event);
	}
		
	m_scene->update();
}

void SceneController::wheelMoved(int wheelDelta) const
{
	m_viewPortWidget->getCurrentCamera()->zoom(wheelDelta);
}

void SceneController::keyPressed(int key) const
{
	auto currentCamera = m_viewPortWidget->getCurrentCamera();
	QVector3D* vec;
	switch (key)
	{
	case Qt::Key_W:
		vec = &currentCamera->getRotation().rotatedVector(QVector3D(0, 0, 8));
		currentCamera->move(vec);
		break;
	case Qt::Key_S:;
		vec = &currentCamera->getRotation().rotatedVector(QVector3D(0, 0, -8));
		currentCamera->move(vec);
		break;
	case Qt::Key_A:
		currentCamera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), -10));
		break;
	case Qt::Key_D:
		currentCamera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 10));
		break;
	case Qt::Key_R:
		currentCamera->move(new QVector3D(0, -2, 0));
		break;
	case Qt::Key_F:
		currentCamera->move(new QVector3D(0, 2, 0));
		break;
	}

	m_viewPortWidget->update();
}

void SceneController::resetCamera() const
{
	m_viewPortWidget->getCurrentCamera()->reset();
	m_viewPortWidget->update();
}

void SceneController::cameraModeSelected()
{
	m_isInCameraMode = true;
}

void SceneController::objectModeSelected()
{
	m_isInCameraMode = false;
}

void SceneController::setTessellation(int t)
{
	m_tessellation = t;
}

void SceneController::mipStateChanged()
{
	m_scene->changeDisplayMode(m_mipState ? SceneModel::DirectRendering : SceneModel::MaximumIntensityProjection);
	m_mipState = !m_mipState;
}

void SceneController::singleViewActivated() const
{
	m_viewPortWidget->singleViewActivated();
}

void SceneController::dualViewActivated() const
{
	m_viewPortWidget->dualViewActivated();
}

void SceneController::quadViewActivated() const
{
	m_viewPortWidget->quadViewActivated();
}

void SceneController::selectedItemChanged(const QModelIndex& current, const QModelIndex& previous) const
{
	m_scene->selectItem(static_cast<SceneItem*>(current.internalPointer()));
}

void SceneController::cubeAdded() const
{
	addItem(OpenGLPrimitiveType::Cube);
}

void SceneController::sphereAdded() const
{
	addItem(OpenGLPrimitiveType::Sphere);
}

void SceneController::cylinderAdded() const
{
	addItem(OpenGLPrimitiveType::Cylinder);
}

void SceneController::coneAdded() const
{
	addItem(OpenGLPrimitiveType::Cone);
}

void SceneController::torusAdded() const
{
	addItem(OpenGLPrimitiveType::Torus);
}
void SceneController::volumeAdded() const
{
	addItem(OpenGLPrimitiveType::Volume);
}

void SceneController::terrainAdded() const
{
	addItem(OpenGLPrimitiveType::Terrain);
}

SceneController::SceneController()
	: m_mipState(false)
	, m_scene(nullptr)
	, m_viewPortWidget(nullptr)
{
	m_lastPos = new QPointF;
	m_isInCameraMode = true;
	m_singleClick = false;
	m_tessellation = 0;
	m_trackBall = new TrackBall(TrackBall::Sphere);
}

void SceneController::addItem(OpenGLPrimitiveType type) const
{
	auto parent = m_scene->getSelectedItem();
	auto position = m_viewPortWidget->getCurrentCamera()->getPointOfInterest();
	auto rigidBodyTransform = new RigidBodyTransformation(*position, QQuaternion());//todo tesselation

	switch (type)
	{
	case Volume: 
		AddVolume(rigidBodyTransform, parent);
		break;
	case Terrain: 
		AddTerrain(rigidBodyTransform, parent);
		break;
	case None:
	case Cube:
	case Sphere:
	case Cylinder:
	case Cone:
	case Torus:
	case Object:
	default:
		m_scene->addItem(type, rigidBodyTransform, parent);
	}
}

void SceneController::AddVolume(RigidBodyTransformation *rigidBodyTransform, SceneItem *parent) const
{
	auto fileName = QFileDialog::getOpenFileName(m_viewPortWidget, tr("Load Volume Data"), "./", tr("Raw Files (*.raw)"));
	if (fileName.isEmpty())
	{
		return;
	}

	auto volume = VolumeModelFactory::createFromFile(fileName);
	volume->setRigidBodyTransform(rigidBodyTransform);
	m_scene->addItem(volume, parent);
	connect(volume->getTransferFunction(), SIGNAL(transferFunctionChanged()), m_viewPortWidget, SLOT(update()));
}

void SceneController::AddTerrain(RigidBodyTransformation* rigidBodyTransform, SceneItem* parent) const
{
	auto heightMapFile = QFileDialog::getOpenFileName(m_viewPortWidget, tr("Load Terrain Data"), "./", tr("Gray Scale Map Files (*.pgm)"));
	if (heightMapFile.isEmpty())
	{
		return;
	}
	
	auto textureFiles = QFileDialog::getOpenFileNames(m_viewPortWidget, tr("Load Material Textures"), "./", tr("JPEG (*.jpg)"));
	if (textureFiles.isEmpty())
	{
		return;
	}

	auto terrain = TerrainModelFactory::createFromFile(heightMapFile, textureFiles);
	terrain->setRigidBodyTransform(rigidBodyTransform);

	m_scene->addItem(terrain, parent);
	connect(terrain, SIGNAL(terrainModelChanged()), m_viewPortWidget, SLOT(update()));
}

void SceneController::manipulateCamera(QPointF screenPos, QVector3D* delta, QMouseEvent* event) const
{
	auto currentCamera = m_viewPortWidget->getCurrentCamera();
	//todo disabled because not needed.
	if (event->buttons() & Qt::LeftButton && !currentCamera->isOrthographic())
	{
		m_trackBall->reset();
		m_trackBall->move(screenPos, m_trackBall->getRotation().conjugate());
		currentCamera->rotate(m_trackBall->getRotation());
		//currentCamera->rotateAroundEye(m_trackBall->getRotation());
	}
	else if (event->buttons() & Qt::RightButton) 
	{
		m_trackBall->release(screenPos, m_trackBall->getRotation().conjugate());
		currentCamera->moveTo(delta);
	}
	else
	{
		m_trackBall->release(screenPos, m_trackBall->getRotation().conjugate());
	}
}

void SceneController::manipulateSelectedItem(QPointF screenPos, QVector3D* delta, QMouseEvent* event) const
{
	auto selectedItem = m_scene->getSelectedItem();
	if (!selectedItem)
	{
		return;
	}

	auto rigidBodyTransform = selectedItem->getRigidBodyTransformation();
	if (event->buttons() & Qt::LeftButton)
	{
		m_trackBall->move(screenPos, m_trackBall->getRotation().conjugate());
		rigidBodyTransform->rotate(m_trackBall->getRotation());
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_trackBall->release(screenPos, m_trackBall->getRotation().conjugate());
		rigidBodyTransform->move(delta);
	}
	else
	{
		m_trackBall->release(screenPos, m_trackBall->getRotation().conjugate());
	}
}
