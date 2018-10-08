#include "image.h"

Image::Image(const char* filename) :
	data_(nullptr)
{
	loadPng(filename);
}

Image::~Image()
{
	if (data_ != nullptr)
	{
		free(data_);
		data_ = nullptr;
	}
}

const void* Image::getData() const
{
	return data_;
}

int Image::getWidth() const
{
	return width_;
}

int Image::getHeight() const
{
	return height_;
}

Image::ColorType Image::getColorType() const
{
	return colorType_;
}

void Image::loadPng(const char* filename)
{
	png_structp png_ptr(nullptr);
	png_infop info_ptr(nullptr);
	unsigned int sig_read(0);
	int color_type;
	int interface_type;
	FILE* fp(nullptr);
	unsigned char* data(nullptr);

	try
	{
		fopen_s(&fp, filename, "rb");
		if (fp == nullptr)
		{
			throw std::exception();
		}

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (png_ptr == nullptr)
		{
			throw std::exception();
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == nullptr)
		{
			throw std::exception();
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			throw std::exception();
		}

		png_init_io(png_ptr, fp);

		png_set_sig_bytes(png_ptr, sig_read);

		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

		png_uint_32 width, height;
		int bit_depth;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interface_type, nullptr, nullptr);

		unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		data = (unsigned char*)malloc(row_bytes * height);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

		for (size_t i = 0; i < height; i++)
		{
			memcpy((data + (row_bytes * (height - 1 - i))), row_pointers[i], row_bytes);
		}

		if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA)
		{
			colorType_ = Image::ColorType::RGBA;
		}
		else
		{
			colorType_ = Image::ColorType::RGB;
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		png_ptr = nullptr;
		info_ptr = nullptr;

		fclose(fp);

		data_ = data;
		width_ = width;
		height_ = height;
	}
	catch (...)
	{
		if (data_ != nullptr)
		{
			free(data_);
			data_ = nullptr;
		}

		if (png_ptr != nullptr)
		{
			if (info_ptr != nullptr)
			{
				png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			}
			else
			{
				png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			}
		}

		if (fp != nullptr)
		{
			fclose(fp);
		}

		throw;
	}
}
