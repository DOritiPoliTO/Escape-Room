#pragma once

#include "../shader.h"
#include "../shaded_model_3d.h"
#include "../directional_light.h"
#include "../../common/matrix44.h"

class ShadowMappingShader :public Shader
{
public:
	std::vector<Matrix44f> depthModelViewProjectionMatrices_;

	ShadowMappingShader(GLsizei shadowMapSize);
	~ShadowMappingShader();

	void execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
		const std::vector<ShadedModel3D*>& pG3DModels, const DirectionalLight* pDirLight);

	const GLuint* getShadowMapIds() const;

private:
	static const float radii[4];

	GLint depthModelViewProjectionMatrixUniformLocation_;

	GLsizei shadowMapSize_;
	GLuint frameBufferIds_[4];
	GLuint shadowMapIds_[4];

	void draw(const ShadedModel3D* pModel);
};
