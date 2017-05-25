
#include "scenecontroller.h"
#include "scenemodel.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include "trackball.h"
#include "volumemodelfactory.h"
#include "rigidbodytransformation.h"

SceneController::~SceneController()
{
	delete m_trackball;
}

SceneController* SceneController::getController()
{
	return s_controller ? s_controller : new SceneController();
}

void SceneController::setModelView(SceneModel* scene, ViewPortWidget* view)
{
	m_viewPortWidget = view;
	m_scene = scene;
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

void SceneController::selectedItemChanged(const QModelIndex& current, const QModelIndex& previous) const
{
	m_scene->selectItem(static_cast<SceneItem*>(current.internalPointer()));
	// auto name = m_scene->getScene()->getSelectedItem()->getName();//todo fix name display
	// m_ui.statusBar->messageChanged(name);
}

void SceneController::cubeAdded()
{
	addObject(OpenGLGeometryType::Cube);
}

void SceneController::sphereAdded()
{
	addObject(OpenGLGeometryType::Sphere);
}

void SceneController::cylinderAdded()
{
	addObject(OpenGLGeometryType::Cylinder);
}

void SceneController::coneAdded()
{
	addObject(OpenGLGeometryType::Cone);
}

void SceneController::torusAdded()
{
	addObject(OpenGLGeometryType::Torus);
}
void SceneController::volumeAdded()
{
	addObject(OpenGLGeometryType::Volume);
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

void SceneController::addObject(OpenGLGeometryType type)
{
	auto parent = m_scene->getSelectedItem();

	if(type == OpenGLGeometryType::Volume)
	{
		auto fileName = QFileDialog::getOpenFileName(m_viewPortWidget, tr("Load Volume Data"), "./", tr("Raw Files (*.raw)"));
		auto volume = VolumeModelFactory::createFromFile(fileName);
	}//todo fix this

	m_scene->addItem(type, new RigidBodyTransformation(), parent);

	//auto selectedItem = m_scene->getSelectedItem();
	//auto cube = new SceneItem(selectedItem); //todo fix init
	//selectedItem->appendChild(cube);
	//todo use tesselation slider value

	
	//m_scene->setVolume(VolumeModelFactory::createFromFile(fileName)); //todo fix volume to tree adding
}
