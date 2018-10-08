#include "shader.h"

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename) :
	id_(0),
	vertexShaderId_(0),
	fragmentShaderId_(0)
{
	char* vertexShaderCode(nullptr);
	char* fragmentShaderCode(nullptr);

	try
	{
		vertexShaderId_ = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);

		vertexShaderCode = readFile(vertexShaderFilename);
		fragmentShaderCode = readFile(fragmentShaderFilename);

		glShaderSource(vertexShaderId_, 1, &vertexShaderCode, NULL);
		glShaderSource(fragmentShaderId_, 1, &fragmentShaderCode, NULL);

		glCompileShader(vertexShaderId_);
		glCompileShader(fragmentShaderId_);

		free(vertexShaderCode);
		vertexShaderCode = nullptr;
		free(fragmentShaderCode);
		fragmentShaderCode = nullptr;

		id_ = glCreateProgram();

		glAttachShader(id_, vertexShaderId_);
		glAttachShader(id_, fragmentShaderId_);

		glLinkProgram(id_);

		GLint success = 0;
		GLint logSize = 0;
		GLchar infoLog[512];
		GLsizei length;
		glGetShaderiv(vertexShaderId_, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetShaderiv(vertexShaderId_, GL_INFO_LOG_LENGTH, &logSize);
			glGetShaderInfoLog(vertexShaderId_, 512, &length, infoLog);

			throw std::exception("Error during run-time compilation of vertex shader!");
		}

		glGetShaderiv(fragmentShaderId_, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderId_, GL_INFO_LOG_LENGTH, &logSize);
			glGetShaderInfoLog(fragmentShaderId_, 512, &length, infoLog);

			throw std::exception("Error during run-time compilation of fragment shader!");
		}
	}
	catch (...)
	{
		if (fragmentShaderCode != nullptr)
		{
			free(fragmentShaderCode);
		}

		if (vertexShaderCode != nullptr)
		{
			free(vertexShaderCode);
		}

		throw;
	}
}

Shader::~Shader()
{
	if (id_ != 0)
	{
		glDetachShader(id_, vertexShaderId_);
		glDetachShader(id_, fragmentShaderId_);

		glDeleteProgram(id_);
		id_ = 0;
	}

	if (vertexShaderId_ != 0)
	{
		glDeleteShader(vertexShaderId_);
		vertexShaderId_ = 0;
	}

	if (fragmentShaderId_ != 0)
	{
		glDeleteShader(fragmentShaderId_);
		fragmentShaderId_ = 0;
	}
}

char* Shader::readFile(const char* shaderFilename)
{
	FILE* fp(nullptr);
	char* content(nullptr);

	try
	{
		if (shaderFilename == nullptr)
		{
			throw std::exception();
		}

		fopen_s(&fp, shaderFilename, "rt");
		if (fp == nullptr)
		{
			throw std::exception();
		}

		fseek(fp, 0, SEEK_END);
		int count = ftell(fp);
		rewind(fp);
		if (count > 0)
		{
			content = (char*)malloc(sizeof(char) * (count + 1));
			if (content == nullptr)
			{
				throw std::exception();
			}

			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}

		fclose(fp);

		return content;
	}
	catch (...)
	{
		if (content != nullptr)
		{
			free(content);
		}

		if (fp != nullptr)
		{
			fclose(fp);
		}

		throw;
	}
}
