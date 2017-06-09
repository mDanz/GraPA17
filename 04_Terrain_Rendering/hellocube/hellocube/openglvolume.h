#pragma once

#include "openglgeometry.h"
#include "volumemodel.h"
#include <QOpenGLFunctions_4_4_Compatibility>


class OpenGLVolume : public OpenGLGeometry
{
public:
	explicit OpenGLVolume();
	virtual ~OpenGLVolume();

	void setModel(VolumeModel *model);
	virtual void draw(QOpenGLShaderProgram *program) override;
private:

	void initVolumeGeometry();

	QOpenGLFunctions_4_4_Compatibility *m_glFunc;

	VolumeModel *m_model;
	QOpenGLTexture *m_dataTexture;
};

