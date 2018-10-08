#pragma once

#include <vector>
#include <GL/glew.h>

class PointLight
{
public:
	GLfloat diffuse_[3];
	GLfloat specular_[3];
	GLfloat position_[3];

	inline PointLight(const GLfloat* diffuse, const GLfloat* specular, const GLfloat* position);

	inline void setPosition(const GLfloat* position);
};

PointLight::PointLight(const GLfloat* diffuse, const GLfloat* specular, const GLfloat* position)
{
	for (int i = 0; i < 3; i++)
	{
		diffuse_[i] = diffuse[i];
		specular_[i] = specular[i];
		position_[i] = position[i];
	}
}

void PointLight::setPosition(const GLfloat* position)
{
	for (size_t i = 0; i < 3; i++)
	{
		position_[i] = position[i];
	}
}
