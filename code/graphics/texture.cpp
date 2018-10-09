#include "texture.h"

Texture::Texture():
	id_(0)
{
	glGenTextures(1, &id_);
}

Texture::Texture(const char* imageFilename, bool useMipmaps):
	Texture()
{
	write(imageFilename, useMipmaps);
}

Texture::Texture(GLsizei width, GLsizei height, GLenum format, const void* pixels, bool useMipmaps):
	id_(0),
	width_(width),
	height_(height),
	format_(format)
{
	glGenTextures(1, &id_);

	glBindTexture(GL_TEXTURE_2D, id_);

	// Copy image data from (CPU) RAM to (GPU) video memory
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (useMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

Texture::~Texture()
{
	if (id_ != 0)
	{
		glDeleteTextures(1, &id_);
		id_ = 0;
	}
}

void Texture::write(const char* imageFilename, bool useMipmaps)
{
	// Load image data in RAM
	Image image(imageFilename);

	// Copy image data from (CPU) RAM to (GPU) video memory
	{
		width_ = image.getWidth();
		height_ = image.getHeight();

		glBindTexture(GL_TEXTURE_2D, id_);

		GLenum format = image.getColorType() == Image::ColorType::RGB ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format,
			GL_UNSIGNED_BYTE, image.getData());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (useMipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}
}

void Texture::write(const void* pixels)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

void Texture::clear(const Color& color)
{
	glClearTexImage(id_, 0, format_, format_, &color);
}

GLuint Texture::getId() const
{
	return id_;
}

GLsizei Texture::getWidth() const
{
	return width_;
}

GLsizei Texture::getHeight() const
{
	return height_;
}

void* Texture::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Texture>();
}

void Texture::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Texture>(ptr);
}
