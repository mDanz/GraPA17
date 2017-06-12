
#include "openglterrain.h"
#include "openglhelper.h"
#include "openglcube.h"
#include "terrainmodel.h"
#include "cameramodel.h"
#include <QOpenGLTexture>

OpenGLTerrain::OpenGLTerrain()
{
	initializeShaderProgram();
	initializeOpenGLFunctions();
	initGeometry();
}

OpenGLTerrain::~OpenGLTerrain()
{
	delete m_terrainProgram;
	m_vertexBuf.destroy();
	m_indexBuf.destroy();
}

void OpenGLTerrain::draw(QOpenGLShaderProgram* program)
{

}

void OpenGLTerrain::draw(TerrainModel& terrain, CameraModel& camera)
{
	if (!terrain.isReady())
	{
		qWarning() << "Terrain is not ready to be displayed!";
		return;
	}

	auto glFunc = OpenGLHelper::getGLFunc();
	m_terrainProgram->bind();

	if (terrain.isWireframeEnabled())
	{
		glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// set the camera uniforms
	m_terrainProgram->setUniformValue("viewMat", *camera.getCameraMatrix());
	m_terrainProgram->setUniformValue("projMat", *camera.getProjectionMatrix());
	m_terrainProgram->setUniformValue("cameraPos", *camera.getPointOfInterest());
	m_terrainProgram->setUniformValue("c_camPos", *camera.getPointOfInterest());// + QVector3D(-GRID_GLOBAL_SCALING/2,0,-GRID_GLOBAL_SCALING/2));

																	  // set the terrain parameter uniforms
	m_terrainProgram->setUniformValue("totalTerrainWidth", terrain.getWidthScale() * m_terrainScaling);
	m_terrainProgram->setUniformValue("terrainWidthScale", static_cast<float>(m_terrainScaling));
	m_terrainProgram->setUniformValue("terrainHeight", terrain.getHeightScale());

	m_terrainProgram->setUniformValue("heightMap", 0);
	m_terrainProgram->setUniformValue("fracture[0]", 1);
	m_terrainProgram->setUniformValue("fracture[1]", 2);
	m_terrainProgram->setUniformValue("fracture[2]", 3);
	m_terrainProgram->setUniformValue("fracture[3]", 4);

	m_terrainProgram->setUniformValue("testSampler", 2);


	// set the fragment shader uniforms
	m_terrainProgram->setUniformValue("frag_viewMat", *(camera.getCameraMatrix()));
	m_terrainProgram->setUniformValue("frag_normalMat", camera.getCameraMatrix()->normalMatrix());

	// bind the heightmap texture
	glFunc->glActiveTexture(GL_TEXTURE0);
	glFunc->glBindTexture(GL_TEXTURE_2D, terrain.getHeighMapTextureName());

	
	// bind the facture textures
	auto materials = terrain.getMaterials();
	QVector<QOpenGLTexture*> materialTextures;
	for (int i = 0; i < materials->size(); i++)
	{
		materialTextures.append(new QOpenGLTexture(QImage(materials->at(i)))); //todo insert warning code
		glFunc->glActiveTexture(GL_TEXTURE1 + i);
		materialTextures.at(i)->bind();
	}

	m_vertexBuf.bind();
	int vertexLocation = m_terrainProgram->attributeLocation("in_position");
	m_terrainProgram->enableAttributeArray(vertexLocation);
	m_terrainProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	m_indexBuf.bind();
	glDrawElements(GL_PATCHES, m_indexBuf.size(), GL_UNSIGNED_SHORT, static_cast<GLvoid*>(nullptr));

	m_terrainProgram->release();

	if (terrain.isWireframeEnabled())
	{
		glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// update the camera height todo refactor this
	float totalWidth = terrain.getWidthScale() * m_terrainScaling;
	auto camHmPos = QPoint((camera.getPosition()->x() / totalWidth + 0.5f) * terrain.getMapSize()->x(),
		(camera.getPosition()->z() / totalWidth + 0.5f) * terrain.getMapSize()->y());

	float height = 2.f + terrain.getHeightScale() * terrain.getHeightValue(camHmPos) / 255.f;
	if (camera.getPosition()->y() < height)
		camera.setHeight(height);


	glFlush();
	//glDrawArrays(GL_PATCHES, 0, m_vertexBuf.size());

	//auto cube = OpenGLCube();
	//cube.draw(program);

	//glFunc->glBegin(GL_PATCHES);
	//glFunc->glVertex3f(0, 0, 0);
	//glFunc->glVertex3f(0, 0, .5);
	//glFunc->glVertex3f(.5, 0, .5);
	//glFunc->glVertex3f(.5, 0, 0);

	//glFunc->glEnd();
}

void OpenGLTerrain::initializeShaderProgram()
{
	m_terrainProgram = OpenGLHelper::createShaderProgram(m_terrain_vshFile, m_terrain_tcsFile, m_terrain_tesFile, m_terrain_fshFile);
	m_terrainProgram->bindAttributeLocation("in_position", 0);
	m_terrainProgram->bindAttributeLocation("in_normal", 1);
	if (!m_terrainProgram->link())
	{
		qWarning() << "Linking Error" << m_terrainProgram->log();
	}
}

void OpenGLTerrain::initGeometry()
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


	m_vertexBuf.create();
	m_indexBuf.create();

	m_vertexBuf.bind();
	m_vertexBuf.allocate(vertices.data(), vertices.size() * sizeof(float));
	m_vertexBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);

	m_indexBuf.bind();
	m_indexBuf.allocate(indices.data(), indices.size() * sizeof(ushort));
	m_indexBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);

	OpenGLHelper::getGLFunc()->glPatchParameteri(GL_PATCH_VERTICES, 4);

	// set up the vao, vbo and ibo
	//
	//glf->glGenBuffers(1, &gridVbo);
	//glf->glGenBuffers(1, &gridIbo);
	//glf->glGenVertexArrays(1, &gridVao);
	//glf->glBindVertexArray(gridVao);

	//glf->glBindBuffer(GL_ARRAY_BUFFER, gridVbo);
	//glf->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	//glf->glEnableVertexAttribArray(0);
	//glf->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex

	//glf->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIbo);
	//glf->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(ushort), indices.data(), GL_STATIC_DRAW);

	//glf->glBindVertexArray(0);

	// set up the tesselation parameters
	//glf->glPatchParameteri(GL_PATCH_VERTICES, 4);





	//m_vertexBuf.create();
	//m_indexBuf.create();

	//auto vertices = new QVector3D[4];
	//vertices[0] = QVector3D(0, 0, 0);
	//vertices[1] = QVector3D(0, 0, 1);
	//vertices[2] = QVector3D(1, 0, 1);
	//vertices[3] = QVector3D(1, 0, 0);

	//auto indices = new GLushort[5];
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 3;

	////auto indices = new GLushort[m_gridSize * m_gridSize * 4];
	///*for (int i = 0; i < m_gridSize; i++)
	//{
	//	for (int j = 0; j < m_gridSize*4; j+=4) 
	//	{
	//		int k = j / 4;
	//		indices[i * m_gridSize + j] = i * m_gridSize + k;
	//		indices[i * m_gridSize + j + 1] = i * m_gridSize + k + 1;
	//		indices[i * m_gridSize + j + 2] = (i + 1) * m_gridSize + k + 1;
	//		indices[i * m_gridSize + j + 3] = (i + 1) * m_gridSize + k;
	//	}
	//}*/


	//m_vertexBuf.bind();
	//m_vertexBuf.allocate(vertices, 4 * sizeof(QVector3D));

	//m_indexBuf.bind();
	//m_indexBuf.allocate(indices, 5 * sizeof(GLushort));

}
