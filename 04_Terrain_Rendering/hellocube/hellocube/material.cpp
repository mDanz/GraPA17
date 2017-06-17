#include "material.h"

Material::Material(GLuint texName, QPointF* heightBounds, QPointF* slopeBounds, float specular)
	: m_textureName(texName)
	, m_heightBounds(heightBounds)
	, m_slopeBounds(slopeBounds)
	, m_specular(0)
{
}

Material::~Material()
{
	delete m_heightBounds;
	delete m_slopeBounds;
}

GLuint Material::getTextureName() const
{
	return m_textureName;
}

float Material::getSpecular() const
{
	return m_specular;
}

QPointF* Material::getHeightBounds() const
{
	return m_heightBounds;
}

QPointF* Material::getSlopeBounds() const
{
	return m_slopeBounds;
}
