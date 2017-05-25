#pragma once

#include <QOpenGLFunctions_4_4_Compatibility>
#include <QOpenGLShaderProgram>

class OpenGLHelper
{
public:
	static QOpenGLFunctions_4_4_Compatibility* getGLFunc();
	static QString Error();
	//static QOpenGLShaderProgram* createShaderProgam(QString vertPath, QString fragPath);
	static void initializeGLFunc(QOpenGLContext *context);

private:
	OpenGLHelper();
	static QString getErrorName(GLenum err);

	static QOpenGLFunctions_4_4_Compatibility* m_glFunc;
};