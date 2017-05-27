
#include "openglhelper.h"
#include <QFileInfo>

QOpenGLFunctions_4_4_Compatibility* OpenGLHelper::m_glFunc = nullptr;

QOpenGLFunctions_4_4_Compatibility* OpenGLHelper::getGLFunc()
{
	return m_glFunc;
}

QString OpenGLHelper::Error()
{
	QString message("");
	GLenum err(m_glFunc->glGetError());
	while (err != GL_NO_ERROR) {
		message += getErrorName(err);
		err = m_glFunc->glGetError();
	}
	return message;
}

QOpenGLShaderProgram* OpenGLHelper::createShaderProgam(QString vshFile, QString fshFile)
{
	auto program = new QOpenGLShaderProgram;
	QOpenGLShader* vertexShader;
	QOpenGLShader* fragmentShader;

	addNewShader(vshFile, QOpenGLShader::Vertex, *program);
	addNewShader(fshFile, QOpenGLShader::Fragment, *program);

	if (!program->link()) 
	{
		qCritical() << "Linking program failed:" << program->log() << "\n";
	}

	return program;
}

void OpenGLHelper::initializeGLFunc(QOpenGLContext *context)
{
	if (m_glFunc)
	{
		return;
	}
	m_glFunc = context->versionFunctions<QOpenGLFunctions_4_4_Compatibility>();
	if (!m_glFunc) {
		qInfo() << "ERROR could not get OpenGLFunctions 4.4 Compatibility!";
	}
	else {
		qInfo() << "OpenGLFunction 4.4 Compatibility ok";
	}
}

OpenGLHelper::OpenGLHelper()
{
}

QString OpenGLHelper::getErrorName(GLenum err)
{
	switch (err) {
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default:
		return {};
	}
}

void OpenGLHelper::addNewShader(QString fileName, QOpenGLShader::ShaderType type, QOpenGLShaderProgram &program)
{
	QFileInfo file(fileName);
	if (file.exists())
	{
		auto shader = new QOpenGLShader(type);
		if (shader->compileSourceFile(fileName))
		{
			program.addShader(shader);
		}
		else
		{
			qCritical() << "Vertex Shader Error" << shader->log();
		}
	}
	else
	{
		qCritical() << "Vertex Shader source file" << fileName << " not found.";
	}
}
