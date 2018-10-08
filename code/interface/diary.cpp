#include "diary.h"
#include "../game.h"

Diary::Diary() :
	pGridBillboard_(nullptr),
	pSelectionBillboard_(nullptr),
	pDirLight_(nullptr),
	pRotateItemText_(nullptr),
	pChooseItemText_(nullptr),
	pDiaryText_(nullptr)
{
	try
	{
		// Initialise components and create resources
		{
			pGridBillboard_ = new Billboard2D("../EscapeRoom/data/textures/diary.png", 1.0f);;

			pSelectionBillboard_ = new Billboard2D("../EscapeRoom/data/textures/selection_box.png",
				0.29f, 0.06f);

			GLfloat ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
			GLfloat diffuse[4] = { 0.5f,0.5f,0.5f,1.0f };
			GLfloat specular[4] = { 0.7f,0.7f,0.7f,1.0f };
			Vector3f lightDirection(-0.2f, 0.0f, -0.8f);
			lightDirection.normalize();
			pDirLight_ = new DirectionalLight(ambient, diffuse, specular, &lightDirection.x);

			pRotateItemText_ = new Text2D("R: Rotate Object", Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt");

			pChooseItemText_ = new Text2D("Arrow Keys: Change Selected Entry",
				Text2D::Alignment::CENTER, "../EscapeRoom/data/fonts/candara.fnt");

			pDiaryText_ = new Text2D("DIARY",
				Text2D::Alignment::CENTER, "../EscapeRoom/data/fonts/scriptina.fnt");
		}

		// Configure components and resources
		{
			Scene& diaryScene = Renderer::getInstance().scenes_[SceneId::DIARY_SCENE];

			diaryScene.add(pGridBillboard_);

			diaryScene.pDirLight_ = pDirLight_;

			diaryScene.camera_.position_ = Vector3f(0.1f, 0.0f, -0.3f);

			pRotateItemText_->position_ = Vector2f(0.4f, -0.8f);
			pRotateItemText_->scale_ = 0.6f;
			pRotateItemText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pRotateItemText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			diaryScene.add(pRotateItemText_);

			pChooseItemText_->position_ = Vector2f(-0.4f, -0.8f);
			pChooseItemText_->scale_ = 0.6f;
			pChooseItemText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pChooseItemText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			diaryScene.add(pChooseItemText_);

			pDiaryText_->color_ = Color(0.0f, 0.0f, 0.0f, 1.0f);
			pDiaryText_->position_ = Vector2f(-0.5f, 0.82f);
			pDiaryText_->scale_ = 1.2f;
			diaryScene.add(pDiaryText_);
		}
	}
	catch (...)
	{
		if (pDiaryText_ != nullptr)
		{
			delete pDiaryText_;
			pDiaryText_ = nullptr;
		}

		if (pChooseItemText_ != nullptr)
		{
			delete pChooseItemText_;
			pChooseItemText_ = nullptr;
		}

		if (pRotateItemText_ != nullptr)
		{
			delete pRotateItemText_;
			pRotateItemText_ = nullptr;
		}

		if (pDirLight_ != nullptr)
		{
			delete pDirLight_;
			pDirLight_ = nullptr;
		}

		if (pSelectionBillboard_ != nullptr)
		{
			delete pSelectionBillboard_;
			pSelectionBillboard_ = nullptr;
		}

		if (pGridBillboard_ != nullptr)
		{
			delete pGridBillboard_;
			pGridBillboard_ = nullptr;
		}

		throw;
	}
}

Diary::~Diary()
{
	if (pDiaryText_ != nullptr)
	{
		delete pDiaryText_;
		pDiaryText_ = nullptr;
	}

	if (pChooseItemText_ != nullptr)
	{
		delete pChooseItemText_;
		pChooseItemText_ = nullptr;
	}

	if (pRotateItemText_ != nullptr)
	{
		delete pRotateItemText_;
		pRotateItemText_ = nullptr;
	}

	if (pDirLight_ != nullptr)
	{
		delete pDirLight_;
		pDirLight_ = nullptr;
	}

	if (pSelectionBillboard_ != nullptr)
	{
		delete pSelectionBillboard_;
		pSelectionBillboard_ = nullptr;
	}

	if (pGridBillboard_ != nullptr)
	{
		delete pGridBillboard_;
		pGridBillboard_ = nullptr;
	}
}

void Diary::open()
{
	if (pEntries_.size() == 0)
	{
		return;
	}

	Scene& diaryScene = Renderer::getInstance().scenes_[SceneId::DIARY_SCENE];

	elapsedTime_ = 0.0f;
	flag_ = true;
	pSelectionBillboard_->colorScale_.a = 0.1f;
	diaryScene.add(pSelectionBillboard_);

	diaryScene.remove(pEntries_[selectedItemIndex_]->getShadedModel3D());

	selectedItemIndex_ = 0;

	diaryScene.add(pEntries_[selectedItemIndex_]->getShadedModel3D());

	pEntries_[selectedItemIndex_]->getShadedModel3D()->angle_ = 0.0f;

	pSelectionBillboard_->position_ = Vector2f(-0.5f, 0.56f * 0.5625f);

	rotateItem_ = true;
}

void Diary::addEntry(Entry* pEntry)
{
	size_t n = pEntries_.size();
	Vector2f position(-0.79f, 0.66f);
	position.y -= n * 0.4f;
	pEntry->getText2D()->position_ = position;
	pEntry->getText2D()->scale_ = 0.75f;
	Renderer::getInstance().scenes_[SceneId::DIARY_SCENE].add(pEntry->getText2D());

	pEntries_.push_back(pEntry);
}

void Diary::update(float deltaTime)
{
	size_t n = pEntries_.size();
	if (n == 0)
	{
		return;
	}

	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	// Process player input
	{
		if (key == 'r')
		{
			rotateItem_ = !rotateItem_;
		}
		else if (key == 103)
		{
			Scene& diaryScene = Renderer::getInstance().scenes_[SceneId::DIARY_SCENE];

			diaryScene.remove(pEntries_[selectedItemIndex_]->getShadedModel3D());

			selectedItemIndex_ = (selectedItemIndex_ + 1) % n;

			diaryScene.add(pEntries_[selectedItemIndex_]->getShadedModel3D());

			pSelectionBillboard_->position_.y = (0.56f - 0.4f * selectedItemIndex_) * 0.5625f;
		}
		else if (key == 101)
		{
			Scene& diaryScene = Renderer::getInstance().scenes_[SceneId::DIARY_SCENE];

			diaryScene.remove(pEntries_[selectedItemIndex_]->getShadedModel3D());

			selectedItemIndex_ = (selectedItemIndex_ - 1 + n) % n;

			diaryScene.add(pEntries_[selectedItemIndex_]->getShadedModel3D());

			pSelectionBillboard_->position_.y = (0.56f - 0.4f * selectedItemIndex_) * 0.5625f;
		}
	}

	// Animate (rotate) 3d model of selected entry
	{
		if (rotateItem_)
		{
			pEntries_[selectedItemIndex_]->getShadedModel3D()->angle_ += deltaTime * 20.0f;
		}
	}

	// Animate (alpha) selection rectangle
	{
		if (flag_)
		{
			elapsedTime_ += deltaTime;

			pSelectionBillboard_->colorScale_.a = 0.1f + smoothStart2(elapsedTime_) * 0.4f;

			if (elapsedTime_ >= 1.0f)
			{
				flag_ = false;
			}
		}
		else
		{
			elapsedTime_ -= deltaTime;

			pSelectionBillboard_->colorScale_.a = 0.1f + smoothStart2(elapsedTime_) * 0.4f;

			if (elapsedTime_ <= 0.0f)
			{
				flag_ = true;
			}
		}
	}
}

Diary::Entry::Entry(const char* G3DModelFilename, const char* textString) :
	pText2D_(nullptr),
	pShadedModel3D_(nullptr)
{
	pShadedModel3D_ = new ShadedModel3D(G3DModelFilename);

	pText2D_ = new Text2D(textString, Text2D::Alignment::LEFT,
		"../EscapeRoom/data/fonts/scriptina.fnt");
	pText2D_->color_ = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

Diary::Entry::~Entry()
{
	if (pText2D_ != nullptr)
	{
		delete pText2D_;
		pText2D_ = nullptr;
	}

	if (pShadedModel3D_ != nullptr)
	{
		delete pShadedModel3D_;
		pShadedModel3D_ = nullptr;
	}
}

ShadedModel3D* Diary::Entry::getShadedModel3D()
{
	return pShadedModel3D_;
}

Text2D* Diary::Entry::getText2D()
{
	return pText2D_;
}
