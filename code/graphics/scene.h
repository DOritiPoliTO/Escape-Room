#pragma once

#include "shaded_model_3d.h"
#include "point_light.h"
#include "directional_light.h"
#include "text_2d.h"
#include "shadeless_model_3d.h"
#include "billboards/billboard_2d.h"
#include "billboards/billboard_3d.h"
#include "camera.h"

class Scene
{
	friend class Renderer;

public:
	PointLight* pPointLight_;
	DirectionalLight* pDirLight_;
	Camera camera_;

	Scene();

	void add(ShadedModel3D* pShadedModel3D);
	void add(ShadelessModel3D* pShadelessModel3D);
	void add(Billboard3D* pBillboard3D);
	void add(Text2D* pText2D);
	void add(Billboard2D* pBillboard2D);
	void remove(ShadedModel3D* pShadedModel3D);
	void remove(ShadelessModel3D* pShadelessModel3D);
	void remove(Billboard3D* pBillboard3D);
	void remove(Text2D* pText2D);
	void remove(Billboard2D* pBillboard2D);

private:
	std::vector<ShadedModel3D*> pShadedModel3Ds_;
	std::vector<ShadelessModel3D*> pShadelessModel3Ds_;
	std::vector<Billboard3D*> pBillboard3Ds_;
	std::vector<Text2D*> pText2Ds_;
	std::vector<Billboard2D*> pBillboard2Ds_;
};
