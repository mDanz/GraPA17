#pragma once

#include "openglwidget.h"
#include "openglprimitivetype.h"

class OpenGLPrimitiveFactory
{
public:
	explicit OpenGLPrimitiveFactory(OpenGLWidget *parent);
	~OpenGLPrimitiveFactory();

	OpenGLGeometry* renderPrimitive(OpenGLPrimitiveType primitiveType);

private:
	OpenGLWidget *m_parent;
	OpenGLGeometry* m_primitives[6];
};

