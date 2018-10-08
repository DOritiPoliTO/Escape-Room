#pragma once

#include <exception>
#include <GL/glew.h>

class Buffer
{
public:
	Buffer(GLenum target);
	Buffer(GLenum target, const void* data, size_t size, GLenum usage);
	~Buffer();

	void write(GLenum target, const void* data, size_t size, GLenum usage);

	GLuint getId() const;
	size_t getSize() const;

private:
	GLuint id_;
	size_t size_;
};
