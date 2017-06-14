
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

OpenGLCube::OpenGLCube()			//todo cleanup this class
	//: m_vertexBuf(QOpenGLBuffer::VertexBuffer)
	//, m_normalBuf(QOpenGLBuffer::VertexBuffer)
	//, m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();
	initGeometry();
	//initCubeGeometry();
}

OpenGLCube::~OpenGLCube()
{
	/*m_vertexBuf.destroy();
	m_normalBuf.destroy();
	m_indexBuf.destroy();*/
}

void OpenGLCube::draw(QOpenGLShaderProgram* program)
{
	renderCube(0); //todo use tesselation
	renderTerrain();


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

void OpenGLCube::renderCube(int tesselation) const
{
	auto glFunc = OpenGLHelper::getGLFunc(); //todo refactor this
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

void OpenGLCube::renderPlane(QVector3D normal, int tesselation, QOpenGLFunctions_4_4_Compatibility *glFunc) const
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

void OpenGLCube::renderTerrain()
{
	auto glFunc = OpenGLHelper::getGLFunc();

	glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glFunc->glBindVertexArray(m_vao);
	glDrawElements(GL_QUADS, m_gridSize * m_gridSize * 4, GL_UNSIGNED_SHORT, static_cast<GLvoid*>(nullptr));

	glFlush();

	qWarning() << "Terrain render log: " + OpenGLHelper::Error();
}
//
//void OpenGLCube::initCubeGeometry()
//{
//	m_vertexBuf.create();
//	m_normalBuf.create();
//	m_indexBuf.create();
//
//	// For cube we would need only 8 vertices but we have to
//	// duplicate vertex for each face because texture coordinate
//	// is different.
//	QVector3D vertices[] = {
//		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
//		QVector3D(1.0f, -1.0f,  1.0f), // v1
//		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
//		QVector3D(1.0f,  1.0f,  1.0f), // v3
//
//		QVector3D(1.0f, -1.0f,  1.0f), // v1
//		QVector3D(1.0f, -1.0f, -1.0f), // v4
//		QVector3D(1.0f,  1.0f,  1.0f), // v3
//		QVector3D(1.0f,  1.0f, -1.0f), // v5
//
//		QVector3D(1.0f, -1.0f, -1.0f), // v4
//		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
//		QVector3D(1.0f,  1.0f, -1.0f), // v5
//		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
//
//		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
//		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
//		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
//		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
//
//		QVector3D(-1.0f, -1.0f, -1.0f),  // v6
//		QVector3D(1.0f, -1.0f, -1.0f), // v4
//		QVector3D(-1.0f, -1.0f,  1.0f),  // v0
//		QVector3D(1.0f, -1.0f,  1.0f), // v1
//
//
//		QVector3D(-1.0f,  1.0f,  1.0f),  // v2
//		QVector3D(1.0f,  1.0f,  1.0f), // v3
//		QVector3D(-1.0f,  1.0f, -1.0f),  // v7
//		QVector3D(1.0f,  1.0f, -1.0f), // v5
//	};
//
//	//QVector3D vertices[] = {
//	//	QVector3D(-.5f, -.5f,  .5f),  // v0
//	//	QVector3D(.5f, -.5f,  .5f), // v1
//	//	QVector3D(-.5f, .5f,  .5f),  // v2
//	//	QVector3D(.5f,  .5f,  .5f), // v3
//
//	//	QVector3D(.5f, -.5f, -.5f), // v4
//	//	QVector3D(.5f,  .5f, -.5f), // v5
//	//	QVector3D(-.5f, -.5f, -.5f),  // v6
//	//	QVector3D(-.5f,  .5f, -.5f),  // v7
//	//};
//
//	QVector3D normals[] = {
//		QVector3D(0, 0, 1),
//		QVector3D(0, 0, 1),
//		QVector3D(0, 0, 1),
//		QVector3D(0, 0, 1),
//
//		QVector3D(1, 0, 0),
//		QVector3D(1, 0, 0),
//		QVector3D(1, 0, 0),
//		QVector3D(1, 0, 0),
//
//		QVector3D(0, 0, -1),
//		QVector3D(0, 0, -1),
//		QVector3D(0, 0, -1),
//		QVector3D(0, 0, -1),
//
//		QVector3D(-1, 0, 0),
//		QVector3D(-1, 0, 0),
//		QVector3D(-1, 0, 0),
//		QVector3D(-1, 0, 0),
//		
//		QVector3D(0, -1, 0),
//		QVector3D(0, -1, 0),
//		QVector3D(0, -1, 0),
//		QVector3D(0, -1, 0),
//
//		QVector3D(0, 1, 0),
//		QVector3D(0, 1, 0),
//		QVector3D(0, 1, 0),
//		QVector3D(0, 1, 0),
//	};
//
//	//QVector3D normals[] = {
//	//	QVector3D(-.5f, -.5f,  .5f),  // v0
//	//	QVector3D(.5f, -.5f,  .5f), // v1
//	//	QVector3D(-.5f, .5f,  .5f),  // v2
//	//	QVector3D(.5f,  .5f,  .5f), // v3
//
//	//	QVector3D(.5f, -.5f, -.5f), // v4
//	//	QVector3D(.5f,  .5f, -.5f), // v5
//	//	QVector3D(-.5f, -.5f, -.5f),  // v6
//	//	QVector3D(-.5f,  .5f, -.5f),  // v7
//	//};
//
//	// Indices for drawing cube faces using triangle strips.
//	// Triangle strips can be connected by duplicating indices
//	// between the strips. If connecting strips have opposite
//	// vertex order then last index of the first strip and first
//	// index of the second strip needs to be duplicated. If
//	// connecting strips have same vertex order then only last
//	// index of the first strip needs to be duplicated.
//	//GLushort indices[] = {
//	//	0, 1, 2, 3, 3,    // Face 0
//	//	1, 1, 4, 3, 5, 5, // Face 1
//	//	4, 4, 6, 5, 7, 7, // Face 2
//	//	6, 6, 0, 7, 2, 2, // Face 3
//	//	6, 6, 4, 0, 1, 1, // Face 4
//	//	2, 2, 3, 7, 5     // Face 5
//	//};
//	//GLushort indices[] = {
//	//	0, 1, 2, 3,    // Face 0
//	//	1, 4, 3, 5, // Face 1
//	//	4, 6, 5, 7, // Face 2
//	//	6, 0, 7, 2, // Face 3
//	//	6, 4, 0, 1, // Face 4
//	//	2, 3, 7, 5     // Face 5
//	//};
//	GLushort indices[] = {
//		0, 1, 2, 3, 3,    // Face 0
//		4, 4, 5, 6, 7, 7, // Face 1
//		8, 8, 9, 10, 11, 11, // Face 2
//		12, 12, 13, 14, 15,15, // Face 3
//		16, 16, 17, 18, 19, 19, // Face 4
//		20, 20, 21, 22, 23     // Face 5
//	};
//
//	m_vertexBuf.bind();
//	m_vertexBuf.allocate(vertices, 6* 4 * sizeof(QVector3D));
//
//	m_normalBuf.bind();
//	m_normalBuf.allocate(normals, 6 * 4 * sizeof(QVector3D));
//
//	m_indexBuf.bind();
//	m_indexBuf.allocate(indices, 24 * sizeof(GLushort));
//
//
//	//m_vao->release();
//}

void OpenGLCube::initGeometry()
{
	QVector<float> vertices;
	for (int z = 0; z <= m_gridSize; z++)
	{
		for (int x = 0; x <= m_gridSize; x++)
		{
			vertices.append(x - m_gridSize / 2.f);
			vertices.append(0.f);
			vertices.append(z - m_gridSize / 2.f);
		}
	}

	QVector<ushort> indices;
	ushort lu, lo, ru, ro;
	for (int z = 0; z<m_gridSize; z++)
	{
		for (int x = 0; x<m_gridSize; x++)
		{
			lu = static_cast<unsigned short>(z * (m_gridSize + 1) + x);
			lo = static_cast<unsigned short>((z + 1) * (m_gridSize + 1) + x);
			ru = static_cast<unsigned short>(z * (m_gridSize + 1) + x + 1);
			ro = static_cast<unsigned short>((z + 1) * (m_gridSize + 1) + x + 1);

			indices.append(lu);
			indices.append(ru);
			indices.append(ro);
			indices.append(lo);
		}
	}


	//m_vertexBuf.create();
	//m_indexBuf.create();
	//
	//m_vertexBuf.bind();
	//m_vertexBuf.allocate(vertices.data(), vertices.size() * sizeof(float));
	//m_vertexBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	//
	//m_indexBuf.bind();
	//m_indexBuf.allocate(indices.data(), indices.size() * sizeof(ushort));
	//m_indexBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	//
	//OpenGLHelper::getGLFunc()->glPatchParameteri(GL_PATCH_VERTICES, 4);

	// set up the vao, vbo and ibo
	auto glFunc = OpenGLHelper::getGLFunc();
	glFunc->glGenVertexArrays(1, &m_vao);
	glFunc->glBindVertexArray(m_vao);
	glFunc->glGenBuffers(1, &m_vertexBuf);
	glFunc->glGenBuffers(1, &m_indexBuf);

	glFunc->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuf);
	glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glFunc->glEnableVertexAttribArray(0);
	glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex

	glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuf);
	glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(ushort), indices.data(), GL_STATIC_DRAW);

	glFunc->glBindVertexArray(0);

	glFunc->glPatchParameteri(GL_PATCH_VERTICES, 4);
}

