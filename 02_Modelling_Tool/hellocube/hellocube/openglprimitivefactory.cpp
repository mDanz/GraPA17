
#include "openglprimitivefactory.h"
#include "openglcube.h"
#include "openglsphere.h"
#include "openglcone.h"
#include "opengltorus.h"
#include "openglcylinder.h"
#include "openglnone.h"

class OpenGLNone;
class OpenGLCube;
class OpenGLSphere;
class OpenGLCylinder;
class OpenGLCone;
class OpenGLTorus;

OpenGLPrimitiveFactory::OpenGLPrimitiveFactory(OpenGLWidget* parent)
	: m_parent(parent) 
{
	m_primitives[0] = new OpenGLNone();
	m_primitives[1] = new OpenGLCube();
	m_primitives[2] = new OpenGLSphere();
	m_primitives[3] = new OpenGLCylinder();
	m_primitives[4] = new OpenGLCone();
	m_primitives[5] = new OpenGLTorus();
}

OpenGLPrimitiveFactory::~OpenGLPrimitiveFactory()
{
	//delete[] m_primitives;
}

OpenGLGeometry* OpenGLPrimitiveFactory::createPrimitive(OpenGLGeometryType primitiveType)
{
	switch (primitiveType)
	{
	case None:
		return m_primitives[0];
	case Cube:
		return m_primitives[1];
	case Sphere:
		return m_primitives[2];
	case Cylinder:
		return m_primitives[3];
	case Cone: 
		return m_primitives[4];
	case Torus:
		return m_primitives[5];
	default: 
		return nullptr;
	}
}
