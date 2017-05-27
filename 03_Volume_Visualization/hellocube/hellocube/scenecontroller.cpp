
#include "scenecontroller.h"
#include "scenemodel.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include "trackball.h"
#include "volumemodelfactory.h"
#include "volumemodel.h"
#include "rigidbodytransformation.h"
#include "cameramodel.h"

SceneController* SceneController::s_controller;

SceneController::~SceneController()
{
	delete m_trackball;
}

SceneController* SceneController::getController()
{
	return s_controller ? s_controller : new SceneController();
}

void SceneController::setModelView(SceneModel* scene, ViewPortWidget* viewPortWidget)
{
	m_viewPortWidget = viewPortWidget;
	m_scene = scene;
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
	// auto name = m_scene->getScene()->getSelectedItem()->getName();//todo fix name display
	// m_ui.statusBar->messageChanged(name);
}

void SceneController::cubeAdded() const
{
	addItem(OpenGLGeometryType::Cube);
}

void SceneController::sphereAdded() const
{
	addItem(OpenGLGeometryType::Sphere);
}

void SceneController::cylinderAdded() const
{
	addItem(OpenGLGeometryType::Cylinder);
}

void SceneController::coneAdded() const
{
	addItem(OpenGLGeometryType::Cone);
}

void SceneController::torusAdded() const
{
	addItem(OpenGLGeometryType::Torus);
}
void SceneController::volumeAdded() const
{
	addItem(OpenGLGeometryType::Volume);
}

SceneController::SceneController()
	: m_scene(nullptr)
	, m_viewPortWidget(nullptr)
{
	m_isInCameraMode = true;
	simpleClick = false;
	m_tessellation = 0;
	m_trackball = new TrackBall(TrackBall::Sphere);
}

void SceneController::addItem(OpenGLGeometryType type) const
{
	auto parent = m_scene->getSelectedItem();
	auto position = m_viewPortWidget->getCurrentCamera()->getPointOfInterest();
	auto rigidBodyTransform = new RigidBodyTransformation(*position, QQuaternion());//todo tesselation

	if (type == OpenGLGeometryType::Volume)
	{
		auto fileName = QFileDialog::getOpenFileName(m_viewPortWidget, tr("Load Volume Data"), "./", tr("Raw Files (*.raw)"));
		auto volume = VolumeModelFactory::createFromFile(fileName);
		volume->setRigidBodyTransform(rigidBodyTransform);
		m_scene->addItem(volume, parent);
	}
	else
	{
		m_scene->addItem(type, rigidBodyTransform, parent);
	}
	
	//m_scene->setVolume(VolumeModelFactory::createFromFile(fileName)); //todo fix volume to tree adding
}
