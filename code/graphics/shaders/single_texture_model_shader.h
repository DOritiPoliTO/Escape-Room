#pragma once

#include <vector>
#include "../shader.h"
#include "../billboards/billboard_2d.h"
#include "../billboards/billboard_3d.h"
#include "../shadeless_model_3d.h"
#include "../shaded_model_3d.h"
#include "../../common/matrix44.h"

class SingleTextureModelShader :public Shader
{
public:
	SingleTextureModelShader();

	void execute(const Matrix44f& perspectiveMatrix, const Vector3f& cameraPosition,
		const Vector3f& cameraDirection, const std::vector<Billboard3D*>& pBillboard3Ds);
	void execute(const Matrix44f& orthographicMatrix, const std::vector<Billboard2D*>& pBillboard2Ds);
	void execute(const Matrix44f& perspectiveMatrix, const Matrix44f& viewMatrix,
		const std::vector<ShadelessModel3D*>& pShadelessModel3Ds);
	void execute(const Matrix44f& perspectiveMatrix, const Vector3f& cameraPosition,
		const Vector3f& cameraDirection, const std::vector<ShadelessModel3D*>& pShadelessModel3Ds);
	void execute(const Matrix44f& perspectiveMatrix, const Matrix44f& viewMatrix,
		const std::vector<ShadedModel3D*>& pShadedModel3Ds);

private:
	GLint modelViewProjectionMatrixUniformLocation_;
	GLint textureSamplerUniformLocation_;
	GLint colorScaleUniformLocation_;

	void draw(Billboard* pBillboard);
	void draw(const ShadelessModel3D* pShadelessModel3D);
	void draw(const ShadedModel3D* pShadedModel3D);
};
