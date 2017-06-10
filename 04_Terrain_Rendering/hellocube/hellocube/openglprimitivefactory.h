#pragma once

#include "openglgeometry.h"
#include "openglprimitivetype.h"

class OpenGLWidget;

class OpenGLPrimitiveFactory
{
public:
	explicit OpenGLPrimitiveFactory(OpenGLWidget *parent = nullptr);
	~OpenGLPrimitiveFactory();

	OpenGLGeometry* renderPrimitive(OpenGLPrimitiveType primitiveType);

private:
	OpenGLWidget *m_parent;
	OpenGLGeometry *m_primitives[8];
};

