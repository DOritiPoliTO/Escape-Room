#include "buffer.h"

Buffer::Buffer(GLenum target) :
	id_(0),
	size_(0)
{
	glGenBuffers(1, &id_);
}

Buffer::Buffer(GLenum target, const void* data, size_t size, GLenum usage) :
	Buffer(target)
{
	write(target, data, size, usage);
}

Buffer::~Buffer()
{
	if (id_ != 0)
	{
		glDeleteBuffers(1, &id_);
		id_ = 0;
	}
}

void Buffer::write(GLenum target, const void* data, size_t size, GLenum usage)
{
	glBindBuffer(target, id_);

	if (size <= size_)
	{
		glBufferSubData(target, 0, size, data);
	}
	else
	{
		size_ = size;

		glBufferData(target, size, data, usage);
	}
}

GLuint Buffer::getId() const
{
	return id_;
}

size_t Buffer::getSize() const
{
	return size_;
}
