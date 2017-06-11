
#include "openglterrain.h"
#include "openglhelper.h"

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
	OpenGLHelper::getGLFunc()->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_vertexBuf.bind();
	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("in_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	m_indexBuf.bind();
	OpenGLHelper::getGLFunc()->glColor3f(1, 0, 0);
	OpenGLHelper::getGLFunc()->glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, m_indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}

void OpenGLTerrain::initCubeGeometry()
{
	m_vertexBuf.create();
	m_indexBuf.create();

	auto vertices = new QVector3D[m_gridSize * m_gridSize];
	auto halfGrid = m_gridSize / 2;
	for (auto i = -halfGrid; i < halfGrid; i++) 
	{
		for (auto j = -halfGrid; j < halfGrid; j++)
		{
			auto k = i * m_step;
			auto l = j * m_step;
			vertices[(i+halfGrid) * m_gridSize + (j+halfGrid)] = QVector3D(k, l, 0);
		}
	}

	auto indices = new GLushort[m_gridSize * m_gridSize * 4];
	for (int i = 0; i < m_gridSize; i++)
	{
		for (int j = 0; j < m_gridSize*4; j+=4) 
		{
			int k = j / 4;
			indices[i * m_gridSize + j] = i * m_gridSize + k;
			indices[i * m_gridSize + j + 1] = i * m_gridSize + k + 1;
			indices[i * m_gridSize + j + 2] = (i + 1) * m_gridSize + k + 1;
			indices[i * m_gridSize + j + 3] = (i + 1) * m_gridSize + k;
		}
	}

	m_vertexBuf.bind();
	m_vertexBuf.allocate(vertices, m_gridSize * m_gridSize * sizeof(QVector3D));

	m_indexBuf.bind();
	m_indexBuf.allocate(indices, m_gridSize * m_gridSize * 4 * sizeof(GLushort));

}
