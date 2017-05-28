
#include "openglvolume.h"
#include "openglhelper.h"
#include <QOpenGLTexture>

OpenGLVolume::OpenGLVolume()
	: m_model(nullptr) 
{
	m_glFunc = OpenGLHelper::getGLFunc();
	initVolumeGeometry();
}

OpenGLVolume::~OpenGLVolume()
{
	if (m_model) 
	{
		delete m_model;
	}
}

void OpenGLVolume::setModel(VolumeModel* model)
{
	m_model = model;
}

void OpenGLVolume::draw(QOpenGLShaderProgram* program)
{
	/*m_dataTexture->bind();
	for (float fIndx = -1.0f; fIndx <= 1.0f; fIndx += 0.003f)
	{
		glBegin(GL_QUADS);
		glTexCoord3f(0.0f, 0.0f, ((float)fIndx + 1.0f) / 2.0f);  \
		glVertex3f(-dViewPortSize, -dViewPortSize, fIndx); \
		glTexCoord3f(1.0f, 0.0f, ((float)fIndx + 1.0f) / 2.0f);  \
		glVertex3f(dViewPortSize, -dViewPortSize, fIndx); \
		glTexCoord3f(1.0f, 1.0f, ((float)fIndx + 1.0f) / 2.0f);  \
		glVertex3f(dViewPortSize, dViewPortSize, fIndx); \
		glTexCoord3f(0.0f, 1.0f, ((float)fIndx + 1.0f) / 2.0f);  \
		glVertex3f(-dViewPortSize, dViewPortSize, fIndx);
		glEnd();
	}*/
}

void OpenGLVolume::initVolumeGeometry()
{
	//unsigned char* chRGBABuffer = new unsigned char[m_model->getDimensions()->z()*m_scene->getDimensions()->y()*m_scene->getDimensions()->x() * 4];


	//glGenTextures(1, (GLuint*)&mu3DTex);

	//Medfile.Read(chBuffer, m_uImageWidth*m_uImageHeight*m_uImageCount);

	/*glBindTexture(GL_TEXTURE_3D, mu3DTex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
*/
	// Convert the data to RGBA data.
	// Here we are simply putting the same value to R, G, B and A channels.
	// Usually for raw data, the alpha value
	// will be constructed by a threshold value given by the user

	//for (int nIndx = 0; nIndx < m_model->getDimensions()->z()*m_scene->getDimensions()->y()*m_scene->getDimensions()->x(); ++nIndx)
	//{
	//	chRGBABuffer[nIndx * 4] = m_model->getData()[nIndx];
	//	chRGBABuffer[nIndx * 4 + 1] = m_model->getData()[nIndx];
	//	chRGBABuffer[nIndx * 4 + 2] = m_model->getData()[nIndx];
	//	chRGBABuffer[nIndx * 4 + 3] = m_model->getData()[nIndx];
	//}

	//m_dataTexture = new QOpenGLTexture(QOpenGLTexture::Target3D);
	//m_dataTexture->allocateStorage(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8);
	//m_dataTexture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8, m_model->getData());


	////glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, m_uImageWidth, m_uImageHeight, m_uImageCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)chRGBABuffer);
	////glBindTexture(GL_TEXTURE_3D, 0);
	//delete[] chRGBABuffer;


}
