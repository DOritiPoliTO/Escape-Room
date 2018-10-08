#pragma once

#include "../common/vector2.h"
#include "../graphics/billboards/billboard_3d.h"

class FireEffect
{
public:
	FireEffect(const char* imageFilename);
	~FireEffect();

	void update(float deltaTime);

	Billboard3D* getBillboard();

private:
	static const float period_;

	Billboard3D* pBillboard3D_;

	int currentRow_;
	int currentColumn_;
	int nRows_;
	int nColumns_;
	float deltaU_;
	float deltaV_;
	float elapsedTime_;

	void computeTexCoords();
};
