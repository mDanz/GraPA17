
#include "openglcube.h"
#include "openglwidget.h"
#include <QtMath>
#include "openglhelper.h"

static const float faceColors[6][3]{
	{ 1, 0, 0 },
	{ 0, 1, 1 },
	{ 1, 0, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 0 },
	{ 0, 1, 0 }
};

OpenGLCube::OpenGLCube()
	: m_vertexBuf(QOpenGLBuffer::VertexBuffer)
	, m_normalBuf(QOpenGLBuffer::VertexBuffer)
	, m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();
	initCubeGeometry();
}

OpenGLCube::~OpenGLCube()
{
	m_vertexBuf.destroy();
	m_normalBuf.destroy();
	m_indexBuf.destroy();
}

void OpenGLCube::draw(QOpenGLShaderProgram* program)
{
	renderCube(0);

	//m_vertexBuf.bind();
	//// Tell OpenGL programmable pipeline how to locate vertex position data
	//int vertexLocation = program->attributeLocation("in_position");
	//program->enableAttributeArray(vertexLocation);
	//program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	//m_normalBuf.bind();
	//int normalLocation = program->attributeLocation("in_normal");
	//program->enableAttributeArray(normalLocation);
	//program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	//m_indexBuf.bind();
	//glDrawElements(GL_TRIANGLE_STRIP, m_indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}

void OpenGLCube::renderCube(int tesselation) 
{
	auto glFunc = OpenGLHelper::getGLFunc();
	glFunc->glBegin(GL_QUADS);
		// front
		renderPlane(QVector3D(0, 0, 1), tesselation, glFunc);
		// right
		renderPlane(QVector3D(1, 0, 0), tesselation, glFunc);
		// back
		renderPlane(QVector3D(0, 0, -1), tesselation, glFunc);
		// left
		renderPlane(QVector3D(-1, 0, 0), tesselation, glFunc);
		// top
		renderPlane(QVector3D(0, 1, 0), tesselation, glFunc);
		// bottom
		renderPlane(QVector3D(0, -1, 0), tesselation, glFunc);
	glFunc->glEnd();
}

void OpenGLCube::renderPlane(QVector3D normal, int tesselation, QOpenGLFunctions_4_4_Compatibility *glFunc)
{
	const QVector3D frontNormal = QVector3D(0., 0., 1.);

	// calculate half the width of the single quads that the plane consists of
	// tesselation = 0 => one quad with width 1 (halfWidth = 0.5)
	// tesselation = 1 => four quads with width 0.5 (halfWidth = 0.25)
	// n>0: tesselation = n => 2^n quads with width 0.5/tesselation (halfWidth = 0.25/tesselation)
	//
	// with phong shading, tesselation can be bypassed
	double halfWidth = (tesselation == 0) ? .5f : .25f / tesselation;

	// vertices for a front plane with width = 2 * halfWidth
	QVector3D v[4] = { QVector3D(halfWidth, halfWidth, 0.5), QVector3D(-halfWidth, halfWidth, 0.5),
		QVector3D(-halfWidth, -halfWidth, 0.5), QVector3D(halfWidth, -halfWidth, 0.5) };


	// calculate the matrix for transforming the vertices from the front plane (normal = (0, 0, 1))
	// to the plane specified by the given normal.
	QMatrix4x4 transform = QMatrix4x4();
	QVector3D axis = QVector3D::crossProduct(frontNormal, normal);
	float angle = qRadiansToDegrees(acos(QVector3D::dotProduct(frontNormal, normal)));
	transform.rotate(angle, axis);

	// use the given normal
	glFunc->glNormal3d(normal.x(), normal.y(), normal.z());

	// walk through the grid and render the 2^tesselation quads
	QVector3D pos;
	for (double y = -0.5 + halfWidth; y < 0.5; y += 2 * halfWidth) {
		for (double x = -0.5 + halfWidth; x < 0.5; x += 2 * halfWidth) {

			// to guarantee that the vertices on the outside of the cube
			// are in CCW order, they have to be added to the pipeline
			// based on the z-orientation of the plane's normal
			if (normal.z() < 0) {
				for (int i = 3; i >= 0; i--) {
					pos = QVector3D(v[i][0] + x, v[i][1] + y, v[i][2]);
					pos = transform * pos;
					glFunc->glVertex3d(pos.x(), pos.y(), pos.z());
				}
			}
			else {
				for (int i = 0; i <= 3; i++) {
					pos = QVector3D(v[i][0] + x, v[i][1] + y, v[i][2]);
					pos = transform * pos;
					glFunc->glVertex3d(pos.x(), pos.y(), pos.z());
				}
			}
		}
	}
}

void OpenGLCube::initCubeGeometry()
{
	m_vertexBuf.create();
	m_normalBuf.create();
	m_indexBuf.create();

	// For cube we would need only 8 vertices but we have to
	// duplicate vertex for each face because texture coordinate
	// is different.
	QVector3D vertices[] = {
		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
		QVector3D(1.0f, -1.0f,  1.0f), // v1
		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
		QVector3D(1.0f,  1.0f,  1.0f), // v3

		QVector3D(1.0f, -1.0f,  1.0f), // v1
		QVector3D(1.0f, -1.0f, -1.0f), // v4
		QVector3D(1.0f,  1.0f,  1.0f), // v3
		QVector3D(1.0f,  1.0f, -1.0f), // v5

		QVector3D(1.0f, -1.0f, -1.0f), // v4
		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
		QVector3D(1.0f,  1.0f, -1.0f), // v5
		QVector3D(-1.0f,  1.0f, -1.0f),  // v7

		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
		QVector3D(-1.0f,  1.0f,  1.0f),  // v2

		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
		QVector3D(1.0f, -1.0f, -1.0f), // v4
		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
		QVector3D(1.0f, -1.0f,  1.0f), // v1


		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
		QVector3D(1.0f,  1.0f,  1.0f), // v3
		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
		QVector3D(1.0f,  1.0f, -1.0f), // v5
	};

	//QVector3D vertices[] = {
	//	QVector3D(-.5f, -.5f,  .5f),  // v0
	//	QVector3D(.5f, -.5f,  .5f), // v1
	//	QVector3D(-.5f, .5f,  .5f),  // v2
	//	QVector3D(.5f,  .5f,  .5f), // v3

	//	QVector3D(.5f, -.5f, -.5f), // v4
	//	QVector3D(.5f,  .5f, -.5f), // v5
	//	QVector3D(-.5f, -.5f, -.5f),  // v6
	//	QVector3D(-.5f,  .5f, -.5f),  // v7
	//};

	QVector3D normals[] = {
		QVector3D(0, 0, 1),
		QVector3D(0, 0, 1),
		QVector3D(0, 0, 1),
		QVector3D(0, 0, 1),

		QVector3D(1, 0, 0),
		QVector3D(1, 0, 0),
		QVector3D(1, 0, 0),
		QVector3D(1, 0, 0),

		QVector3D(0, 0, -1),
		QVector3D(0, 0, -1),
		QVector3D(0, 0, -1),
		QVector3D(0, 0, -1),

		QVector3D(-1, 0, 0),
		QVector3D(-1, 0, 0),
		QVector3D(-1, 0, 0),
		QVector3D(-1, 0, 0),
		
		QVector3D(0, -1, 0),
		QVector3D(0, -1, 0),
		QVector3D(0, -1, 0),
		QVector3D(0, -1, 0),

		QVector3D(0, 1, 0),
		QVector3D(0, 1, 0),
		QVector3D(0, 1, 0),
		QVector3D(0, 1, 0),
	};

	//QVector3D normals[] = {
	//	QVector3D(-.5f, -.5f,  .5f),  // v0
	//	QVector3D(.5f, -.5f,  .5f), // v1
	//	QVector3D(-.5f, .5f,  .5f),  // v2
	//	QVector3D(.5f,  .5f,  .5f), // v3

	//	QVector3D(.5f, -.5f, -.5f), // v4
	//	QVector3D(.5f,  .5f, -.5f), // v5
	//	QVector3D(-.5f, -.5f, -.5f),  // v6
	//	QVector3D(-.5f,  .5f, -.5f),  // v7
	//};

	// Indices for drawing cube faces using triangle strips.
	// Triangle strips can be connected by duplicating indices
	// between the strips. If connecting strips have opposite
	// vertex order then last index of the first strip and first
	// index of the second strip needs to be duplicated. If
	// connecting strips have same vertex order then only last
	// index of the first strip needs to be duplicated.
	//GLushort indices[] = {
	//	0, 1, 2, 3, 3,    // Face 0
	//	1, 1, 4, 3, 5, 5, // Face 1
	//	4, 4, 6, 5, 7, 7, // Face 2
	//	6, 6, 0, 7, 2, 2, // Face 3
	//	6, 6, 4, 0, 1, 1, // Face 4
	//	2, 2, 3, 7, 5     // Face 5
	//};
	//GLushort indices[] = {
	//	0, 1, 2, 3,    // Face 0
	//	1, 4, 3, 5, // Face 1
	//	4, 6, 5, 7, // Face 2
	//	6, 0, 7, 2, // Face 3
	//	6, 4, 0, 1, // Face 4
	//	2, 3, 7, 5     // Face 5
	//};
	GLushort indices[] = {
		0, 1, 2, 3, 3,    // Face 0
		4, 4, 5, 6, 7, 7, // Face 1
		8, 8, 9, 10, 11, 11, // Face 2
		12, 12, 13, 14, 15,15, // Face 3
		16, 16, 17, 18, 19, 19, // Face 4
		20, 20, 21, 22, 23     // Face 5
	};

	m_vertexBuf.bind();
	m_vertexBuf.allocate(vertices, 6* 4 * sizeof(QVector3D));

	m_normalBuf.bind();
	m_normalBuf.allocate(normals, 6 * 4 * sizeof(QVector3D));

	m_indexBuf.bind();
	m_indexBuf.allocate(indices, 24 * sizeof(GLushort));


	//m_vao->release();
}
