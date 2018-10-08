#include "text_2d_shader.h"

Text2DShader::Text2DShader() :
	Shader("../EscapeRoom/data/shaders/text_2d_shader.vert",
		"../EscapeRoom/data/shaders/text_2d_shader.frag")
{
	positionUniformLocation_ = glGetUniformLocation(id_, "u_Position");
	textureSamplerUniformLocation_ = glGetUniformLocation(id_, "u_TextureSampler");
	colorUniformLocation_ = glGetUniformLocation(id_, "u_Color");
	scaleUniformLocation_ = glGetUniformLocation(id_, "u_Scale");
	aspectRatioUniformLocation_ = glGetUniformLocation(id_, "u_AspectRatio");
	effectTypeUniformLocation_ = glGetUniformLocation(id_, "u_EffectType");
	effectScaleUniformLocation_ = glGetUniformLocation(id_, "u_EffectScale");
	effectColorUniformLocation_ = glGetUniformLocation(id_, "u_EffectColor");
}

void Text2DShader::execute(GLfloat aspectRatio, const std::vector<Text2D*>& pText2Ds)
{
	glUseProgram(id_);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(aspectRatioUniformLocation_, aspectRatio);

	for each (auto pText2D in pText2Ds)
	{
		glUniform2fv(positionUniformLocation_, 1, &pText2D->position_.x);

		glUniform4fv(colorUniformLocation_, 1, &pText2D->color_.r);

		glUniform1f(scaleUniformLocation_, pText2D->scale_);

		glUniform1i(effectTypeUniformLocation_, pText2D->effectType_);

		glUniform1f(effectScaleUniformLocation_, pText2D->effectScale_);

		glUniform3fv(effectColorUniformLocation_, 1, &pText2D->effectColor_.r);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pText2D->getFont()->getTexture().getId());
		glUniform1i(textureSamplerUniformLocation_, 0);

		glBindBuffer(GL_ARRAY_BUFFER, pText2D->getVertexBuffer().getId());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Text2D::VertexAttribs),
			(void*)offsetof(Text2D::VertexAttribs, position)
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Text2D::VertexAttribs),
			(void*)offsetof(Text2D::VertexAttribs, texCoord)
		);

		glDrawArrays(
			GL_QUADS,
			0,
			pText2D->getVertexBuffer().getSize() / sizeof(Text2D::VertexAttribs)
		);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}

	glDisable(GL_BLEND);
}
