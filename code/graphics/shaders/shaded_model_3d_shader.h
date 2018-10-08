#pragma once

#include "../shader.h"
#include "../shaded_model_3d.h"
#include "../point_light.h"
#include "../directional_light.h"
#include "../../common/matrix44.h"
#include "../../common/frustum.h"

class ShadedModel3DShader :public Shader
{
public:
	ShadedModel3DShader();

	void execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
		const std::vector<ShadedModel3D*>& pShadedModel3Ds, const PointLight* pPointLight,
		GLuint pointLightShadowMapId, const DirectionalLight* pDirLight,
		const GLuint* dirLightShadowMapIds, const std::vector<Matrix44f>& depthModelViewProjectionMatrices,
		GLuint environmentMapId, const Matrix44f& projectionMatrix, const Frustum& viewFrustum);

private:
	GLint viewMatrixUniformLocation_;
	GLint modelViewMatrixUniformLocation_;
	GLint projectionMatrixUniformLocation_;
	GLint modelMatrixUniformLocation_;
	GLint depthModelViewProjectionMatrixUniformLocations_[4];
	GLint pointLightPosition_WS_UniformLocation_;
	GLint dirLightDirection_WS_UniformLocation_;
	GLint shadowMapCubeSamplerUniformLocation_;
	GLint shadowMapSamplerUniformLocations_[4];
	GLint isDirLightOnUniformLocation_;
	GLint dirLightAmbientUniformLocation_;
	GLint dirLightDiffuseUniformLocation_;
	GLint dirLightSpecularUniformLocation_;
	GLint isPointLightOnUniformLocation_;
	GLint pointLightDiffuseUniformLocation_;
	GLint pointLightSpecularUniformLocation_;
	GLint normalTextureSamplerUniformLocation_;
	GLint diffuseColorTextureSamplerUniformLocation_;
	GLint specularTextureSamplerUniformLocation_;
	GLint environmentMapCubeSamplerUniformLocation_;
	GLint cameraPosition_WS_UniformLocation_;
	GLint materialReflectivityUniformLocation_;

	void draw(const ShadedModel3D* pShadedModel3D);
};
