#pragma once

#include <exception>
#include <memory>
#include <png.h>

class Image
{
public:
	enum ColorType;

	Image(const char* filename);
	~Image();

	const void* getData() const;
	int getWidth() const;
	int getHeight() const;
	ColorType getColorType() const;

private:
	void* data_;
	int width_;
	int height_;
	ColorType colorType_;

	void loadPng(const char* filename);
};

enum Image::ColorType
{
	RGB,
	RGBA
};
