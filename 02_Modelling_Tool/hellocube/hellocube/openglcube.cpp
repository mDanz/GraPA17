
#include "openglcube.h"

OpenGLCube::OpenGLCube()
	: m_vertexBuf(QOpenGLBuffer::VertexBuffer)
	, m_normalBuf(QOpenGLBuffer::VertexBuffer)
	, m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();

	m_vertexBuf.create();
	m_normalBuf.create();
	m_indexBuf.create();

	initCubeGeometry();
}

OpenGLCube::~OpenGLCube()
{
	m_vertexBuf.destroy();
	m_normalBuf.destroy();
	m_indexBuf.destroy();
}

void OpenGLCube::drawCubeGeometry(QOpenGLShaderProgram* program)
{
	m_vertexBuf.bind();


	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("in_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));


	m_normalBuf.bind();
	int normalLocation = program->attributeLocation("in_normal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	m_indexBuf.bind();
	// Draw cube geometry using indices from VBO 1
	glDrawElements(GL_TRIANGLE_STRIP, m_indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}

void OpenGLCube::initCubeGeometry()
{
	// For cube we would need only 8 vertices but we have to
	// duplicate vertex for each face because texture coordinate
	// is different.
	QVector3D vertices[] = {
		// Vertex data for face 0
		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
		QVector3D(1.0f, -1.0f,  1.0f), // v1
		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
		QVector3D(1.0f,  1.0f,  1.0f), // v3
		QVector3D(1.0f, -1.0f, -1.0f), // v4
		QVector3D(1.0f,  1.0f, -1.0f), // v5
		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
	};

	QVector3D normals[] = {
		QVector3D(0, 0, 1),
		QVector3D(1, 0, 0),
		QVector3D(0, 0, -1),
		QVector3D(-1, 0, 0),
		QVector3D(0, -1, 0),
		QVector3D(0, 1, 0)
	};

	// Indices for drawing cube faces using triangle strips.
	// Triangle strips can be connected by duplicating indices
	// between the strips. If connecting strips have opposite
	// vertex order then last index of the first strip and first
	// index of the second strip needs to be duplicated. If
	// connecting strips have same vertex order then only last
	// index of the first strip needs to be duplicated.
	GLushort indices[] = {
		0, 1, 2, 3, 3,    // Face 0
		1, 1, 4, 3, 5, 5, // Face 1
		4, 4, 6, 5, 7, 7, // Face 2
		6, 6, 0, 7, 2, 2, // Face 3
		6, 6, 4, 0, 1, 1, // Face 4
		2, 2, 3, 7, 5     // Face 5
	};

	m_vertexBuf.bind();
	m_vertexBuf.allocate(vertices, 8 * sizeof(QVector3D));

	m_normalBuf.bind();
	m_normalBuf.allocate(normals, 6 * sizeof(QVector3D));

	m_indexBuf.bind();
	m_indexBuf.allocate(indices, 34 * sizeof(GLushort));

}
