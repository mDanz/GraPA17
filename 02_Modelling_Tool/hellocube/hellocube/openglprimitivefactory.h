#pragma once

#include "openglwidget.h"
#include "openglgeometrytype.h"

class OpenGLPrimitiveFactory
{
public:
	explicit OpenGLPrimitiveFactory(OpenGLWidget *parent);
	~OpenGLPrimitiveFactory();

	OpenGLGeometry* createPrimitive(OpenGLGeometryType primitiveType);

private:
	OpenGLWidget *m_parent;
	OpenGLGeometry* m_primitives[6];
};

