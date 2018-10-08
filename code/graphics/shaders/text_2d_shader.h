#pragma once

#include <vector>
#include "../shader.h"
#include "../text_2d.h"

class Text2DShader :public Shader
{
public:
	Text2DShader();

	void execute(GLfloat aspectRatio, const std::vector<Text2D*>& pText2Ds);

private:
	GLint positionUniformLocation_;
	GLint textureSamplerUniformLocation_;
	GLint colorUniformLocation_;
	GLint scaleUniformLocation_;
	GLint aspectRatioUniformLocation_;
	GLint effectTypeUniformLocation_;
	GLint effectScaleUniformLocation_;
	GLint effectColorUniformLocation_;
};
