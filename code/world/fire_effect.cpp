#include "fire_effect.h"

const float FireEffect::period_ = 0.04f;

FireEffect::FireEffect(const char* imageFilename) :
	pBillboard3D_(nullptr),
	elapsedTime_(0.0f),
	currentColumn_(0),
	currentRow_(0),
	nColumns_(12),
	nRows_(6),
	deltaU_(0.083333333f),
	deltaV_(0.166666666f)
{
	try
	{
		GLfloat vertices[]{
			-0.02f, 0.04f, 0.0f,
			0.02f, 0.04f, 0.0f,
			0.02f, -0.0f, 0.0f,
			-0.02f, -0.0f, 0.0f
		};

		GLfloat texCoords[]{
			0.0f, 1.0f,
			deltaU_, 1.0f,
			deltaU_, 1.0f - deltaV_,
			0.0f, 1.0f - deltaV_
		};

		pBillboard3D_ = new Billboard3D(imageFilename, vertices, texCoords);
	}
	catch (...)
	{
		if (pBillboard3D_ != nullptr)
		{
			delete pBillboard3D_;
			pBillboard3D_ = nullptr;
		}

		throw;
	}
}

FireEffect::~FireEffect()
{
	if (pBillboard3D_ != nullptr)
	{
		delete pBillboard3D_;
		pBillboard3D_ = nullptr;
	}
}

void FireEffect::update(float deltaTime)
{
	elapsedTime_ += deltaTime;
	if (elapsedTime_ < period_)
	{
		return;
	}

	elapsedTime_ -= period_;

	currentColumn_++;
	if (currentColumn_ == nColumns_)
	{
		currentColumn_ = 0;

		currentRow_++;
		if (currentRow_ == nRows_)
		{
			currentRow_ = 0;
		}
	}

	computeTexCoords();
}

Billboard3D* FireEffect::getBillboard()
{
	return pBillboard3D_; 
}

void FireEffect::computeTexCoords()
{
	Vector2f topLeft = { 0.0f,1.0f };
	Vector2f topRight = topLeft + Vector2f(deltaU_, 0.0f);
	Vector2f bottomRight = topRight - Vector2f(0.0f, deltaV_);
	Vector2f bottomLeft = bottomRight - Vector2f(deltaU_, 0.0f);
	Vector2f texCoords[4];

	Vector2f deltaTexCoord(currentColumn_ * deltaU_, currentRow_ * -deltaV_);

	texCoords[0] = topLeft + deltaTexCoord;
	texCoords[1] = topRight + deltaTexCoord;
	texCoords[2] = bottomRight + deltaTexCoord;
	texCoords[3] = bottomLeft + deltaTexCoord;

	pBillboard3D_->getTexCoordBuffer().write(GL_ARRAY_BUFFER, texCoords, 4 * sizeof(Vector2f),
		GL_STATIC_DRAW);
}
