
#include "openglvolume.h"
#include "openglhelper.h"

OpenGLVolume::OpenGLVolume() //todo move render stuff from OpenGLWidget here.
	: m_model(nullptr) 
{
	m_glFunc = OpenGLHelper::getGLFunc();
	initVolumeGeometry();
}

OpenGLVolume::~OpenGLVolume()
{
	if (m_model) 
	{
		delete m_model;
	}
}

void OpenGLVolume::setModel(VolumeModel* model)
{
	m_model = model;
}

void OpenGLVolume::draw(QOpenGLShaderProgram* program)
{
}

void OpenGLVolume::initVolumeGeometry()
{
}
