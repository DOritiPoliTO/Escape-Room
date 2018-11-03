#pragma once

#include <vector>
#include <GL/glew.h>

class DirectionalLight
{
public:
	GLfloat ambient_[3];
	GLfloat diffuse_[3];
	GLfloat specular_[3];
	GLfloat direction_[3];

	DirectionalLight(const GLfloat* ambient, const GLfloat* diffuse, const GLfloat* specular,
		const GLfloat* direction);

	void setDirection(const GLfloat* direction);
};

inline DirectionalLight::DirectionalLight(const GLfloat* ambient, const GLfloat* diffuse, const GLfloat* specular,
	const GLfloat* direction)
{
	for (int i = 0; i < 3; i++)
	{
		ambient_[i] = ambient[i];
		diffuse_[i] = diffuse[i];
		specular_[i] = specular[i];
		direction_[i] = direction[i];
	}
}

inline void DirectionalLight::setDirection(const GLfloat* direction)
{
	for (size_t i = 0; i < 3; i++)
	{
		direction_[i] = direction[i];
	}
}
