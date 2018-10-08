#include "scene.h"

Scene::Scene()	:
	pPointLight_(nullptr),
	pDirLight_(nullptr)
{
}

void Scene::add(ShadedModel3D* pShadedModel3D)
{
	auto it = pShadedModel3Ds_.begin();
	for (; it != pShadedModel3Ds_.end(); it++)
	{
		if ((*it) == pShadedModel3D)
		{
			break;
		}
	}

	if (it == pShadedModel3Ds_.end())
	{
		pShadedModel3Ds_.push_back(pShadedModel3D);
	}
}

void Scene::add(ShadelessModel3D* pShadelessModel3D)
{
	auto it = pShadelessModel3Ds_.begin();
	for (; it != pShadelessModel3Ds_.end(); it++)
	{
		if ((*it) == pShadelessModel3D)
		{
			break;
		}
	}

	if (it == pShadelessModel3Ds_.end())
	{
		pShadelessModel3Ds_.push_back(pShadelessModel3D);
	}
}

void Scene::add(Billboard3D* pBillboard3D)
{
	auto it = pBillboard3Ds_.begin();
	for (; it != pBillboard3Ds_.end(); it++)
	{
		if ((*it) == pBillboard3D)
		{
			break;
		}
	}

	if (it == pBillboard3Ds_.end())
	{
		pBillboard3Ds_.push_back(pBillboard3D);
	}
}

void Scene::add(Text2D* pText2D)
{
	auto it = pText2Ds_.begin();
	for (; it != pText2Ds_.end(); it++)
	{
		if ((*it) == pText2D)
		{
			break;
		}
	}

	if (it == pText2Ds_.end())
	{
		pText2Ds_.push_back(pText2D);
	}
}

void Scene::add(Billboard2D* pBillboard2D)
{
	auto it = pBillboard2Ds_.begin();
	for (; it != pBillboard2Ds_.end(); it++)
	{
		if ((*it) == pBillboard2D)
		{
			break;
		}
	}

	if (it == pBillboard2Ds_.end())
	{
		pBillboard2Ds_.push_back(pBillboard2D);
	}
}

void Scene::remove(ShadedModel3D* pShadedModel3D)
{
	auto it = pShadedModel3Ds_.begin();
	for (; it != pShadedModel3Ds_.end(); it++)
	{
		if ((*it) == pShadedModel3D)
		{
			break;
		}
	}

	if (it != pShadedModel3Ds_.end())
	{
		pShadedModel3Ds_.erase(it);
	}
}

void Scene::remove(ShadelessModel3D* pShadelessModel3D)
{
	auto it = pShadelessModel3Ds_.begin();
	for (; it != pShadelessModel3Ds_.end(); it++)
	{
		if ((*it) == pShadelessModel3D)
		{
			break;
		}
	}

	if (it != pShadelessModel3Ds_.end())
	{
		pShadelessModel3Ds_.erase(it);
	}
}

void Scene::remove(Billboard3D* pBillboard3D)
{
	auto it = pBillboard3Ds_.begin();
	for (; it != pBillboard3Ds_.end(); it++)
	{
		if ((*it) == pBillboard3D)
		{
			break;
		}
	}

	if (it != pBillboard3Ds_.end())
	{
		pBillboard3Ds_.erase(it);
	}
}

void Scene::remove(Text2D* pText2D)
{
	auto it = pText2Ds_.begin();
	for (; it != pText2Ds_.end(); it++)
	{
		if ((*it) == pText2D)
		{
			break;
		}
	}

	if (it != pText2Ds_.end())
	{
		pText2Ds_.erase(it);
	}
}

void Scene::remove(Billboard2D* pBillboard2D)
{
	auto it = pBillboard2Ds_.begin();
	for (; it != pBillboard2Ds_.end(); it++)
	{
		if ((*it) == pBillboard2D)
		{
			break;
		}
	}

	if (it != pBillboard2Ds_.end())
	{
		pBillboard2Ds_.erase(it);
	}
}
