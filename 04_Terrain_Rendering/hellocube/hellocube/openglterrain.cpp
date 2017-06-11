
#include "openglterrain.h"
#include "openglhelper.h"
#include "openglcube.h"

OpenGLTerrain::OpenGLTerrain()
{
	initializeOpenGLFunctions();
	initCubeGeometry();
}

OpenGLTerrain::~OpenGLTerrain()
{
	m_vertexBuf.destroy();
	m_indexBuf.destroy();
}

void OpenGLTerrain::draw(QOpenGLShaderProgram* program)
{
	auto glFunc = OpenGLHelper::getGLFunc();
	glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//m_vertexBuf.bind();
	//// Tell OpenGL programmable pipeline how to locate vertex position data
	//int vertexLocation = program->attributeLocation("in_position");
	//program->enableAttributeArray(vertexLocation);
	//program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	//m_indexBuf.bind();
	glFunc->glPatchParameteri(GL_PATCH_VERTICES, 4);
	//glDrawElements(GL_PATCHES, m_indexBuf.size(), GL_UNSIGNED_SHORT, 0);
	//glDrawArrays(GL_PATCHES, 0, m_vertexBuf.size());

	//auto cube = OpenGLCube();
	//cube.draw(program);

	glFunc->glBegin(GL_PATCHES);
	glFunc->glVertex3f(0, 0, 0);
	glFunc->glVertex3f(0, 0, .5);
	glFunc->glVertex3f(.5, 0, .5);
	glFunc->glVertex3f(.5, 0, 0);

	glFunc->glEnd();
}

void OpenGLTerrain::initCubeGeometry()
{
	m_vertexBuf.create();
	m_indexBuf.create();

	//auto vertices = new QVector3D[m_gridSize * m_gridSize];
	//auto halfGrid = m_gridSize / 2;
	/*for (auto i = -halfGrid; i < halfGrid; i++) 
	{
		for (auto j = -halfGrid; j < halfGrid; j++)
		{
			auto k = i * m_step;
			auto l = j * m_step;
			vertices[(i+halfGrid) * m_gridSize + (j+halfGrid)] = QVector3D(k, l, 0);
		}
	}*/

	auto vertices = new QVector3D[4];
	vertices[0] = QVector3D(0, 0, 0);
	vertices[1] = QVector3D(0, 0, 1);
	vertices[2] = QVector3D(1, 0, 1);
	vertices[3] = QVector3D(1, 0, 0);

	auto indices = new GLushort[5];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	//auto indices = new GLushort[m_gridSize * m_gridSize * 4];
	/*for (int i = 0; i < m_gridSize; i++)
	{
		for (int j = 0; j < m_gridSize*4; j+=4) 
		{
			int k = j / 4;
			indices[i * m_gridSize + j] = i * m_gridSize + k;
			indices[i * m_gridSize + j + 1] = i * m_gridSize + k + 1;
			indices[i * m_gridSize + j + 2] = (i + 1) * m_gridSize + k + 1;
			indices[i * m_gridSize + j + 3] = (i + 1) * m_gridSize + k;
		}
	}*/


	m_vertexBuf.bind();
	m_vertexBuf.allocate(vertices, 4 * sizeof(QVector3D));

	m_indexBuf.bind();
	m_indexBuf.allocate(indices, 5 * sizeof(GLushort));

}
