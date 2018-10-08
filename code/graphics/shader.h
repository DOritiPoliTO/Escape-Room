#pragma once

#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

class Shader
{
private:
	char* readFile(const char* shaderFilename);

protected:
	GLuint id_;
	GLuint vertexShaderId_;
	GLuint fragmentShaderId_;

	Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~Shader();
};
