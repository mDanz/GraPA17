#pragma once
#include <QOpenGLVersionFunctions>
#include <qpoint.h>


class Material 
{
public:
	explicit Material(GLuint texName, QPointF* heightBounds = nullptr, QPointF* slopeBounds = nullptr, float specular = 0.f);
	~Material();

	GLuint getTextureName() const;
	float getSpecular() const;
	QPointF* getHeightBounds() const;
	QPointF* getSlopeBounds() const;

private:
	GLuint m_textureName;
	QPointF* m_heightBounds;
	QPointF* m_slopeBounds;
	float m_specular;
};
