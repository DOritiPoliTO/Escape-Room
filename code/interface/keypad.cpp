#include "keypad.h"
#include "../game.h"

Keypad::Keypad() :
	pFailSound_(nullptr),
	pSuccessSound_(nullptr),
	pCombinationText_(nullptr),
	pUndoCmdText_(nullptr),
	pConfirmCmdText_(nullptr)
{
	try
	{
		// Create resources
		{
			pCombinationText_ = new Text2D("../EscapeRoom/data/fonts/candara.fnt");

			pUndoCmdText_ = new Text2D("Del: Cancel", Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt");

			pConfirmCmdText_ = new Text2D("Enter: Confirm", Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt");

			pFailSound_ = AudioSystem::getInstance().loadSound(
				"../EscapeRoom/data/sounds/fail_sound.wav", false);
			pSuccessSound_ = AudioSystem::getInstance().loadSound(
				"../EscapeRoom/data/sounds/success_sound.wav", false);
		}

		// Configure resources
		{
			pCombinationText_->position_ = Vector2f(0.5f, 0.25f);
			pCombinationText_->scale_ = 1.5f;
			pCombinationText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pCombinationText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE].add(pCombinationText_);

			pUndoCmdText_->position_ = Vector2f(-0.99f, 0.92f);
			pUndoCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pUndoCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);

			pConfirmCmdText_->position_ = Vector2f(-0.99f, 0.84f);
			pConfirmCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pConfirmCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
		}
	}
	catch (...)
	{
		if (pSuccessSound_ != nullptr)
		{
			delete pSuccessSound_;
			pSuccessSound_ = nullptr;
		}

		if (pFailSound_ != nullptr)
		{
			delete pFailSound_;
			pFailSound_ = nullptr;
		}

		if (pConfirmCmdText_ != nullptr)
		{
			delete pConfirmCmdText_;
			pConfirmCmdText_ = nullptr;
		}

		if (pUndoCmdText_ != nullptr)
		{
			delete pUndoCmdText_;
			pUndoCmdText_ = nullptr;
		}

		if (pCombinationText_ != nullptr)
		{
			delete pCombinationText_;
			pCombinationText_ = nullptr;
		}

		throw;
	}
}

Keypad::~Keypad()
{
	if (pSuccessSound_ != nullptr)
	{
		delete pSuccessSound_;
		pSuccessSound_ = nullptr;
	}

	if (pFailSound_ != nullptr)
	{
		delete pFailSound_;
		pFailSound_ = nullptr;
	}

	if (pConfirmCmdText_ != nullptr)
	{
		delete pConfirmCmdText_;
		pConfirmCmdText_ = nullptr;
	}

	if (pUndoCmdText_ != nullptr)
	{
		delete pUndoCmdText_;
		pUndoCmdText_ = nullptr;
	}

	if (pCombinationText_ != nullptr)
	{
		delete pCombinationText_;
		pCombinationText_ = nullptr;
	}
}

bool Keypad::update(float deltaTime)
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 27)
	{
		callback_(nullptr);

		return true;
	}

	if (nInsert_ != nTot_)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= 1.0f)
		{
			int index = nInsert_ * 2;
			if (string_[index] == '_')
			{
				string_[index] = ' ';
			}
			else if (string_[index] == ' ')
			{
				string_[index] = '_';
			}
			pCombinationText_->setString(string_, Text2D::Alignment::LEFT);

			elapsedTime_ -= 1.0f;
		}
	}

	if (key == -1)
	{
		return false;
	}

	Scene& keypadScene = Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE];

	// If the player has pressed the "delete" key, delete last number
	if (key == 8)
	{
		if (nInsert_ != 0)
		{
			if (nInsert_ == nTot_)
			{
				keypadScene.remove(pConfirmCmdText_);
			}

			nInsert_--;

			string_[nInsert_ * 2] = '_';
			pCombinationText_->setString(string_, Text2D::Alignment::LEFT);

			pCombinationText_->color_ = Color(1.0f, 1.0f, 1.0f);

			if (nInsert_ == 0)
			{
				keypadScene.remove(pUndoCmdText_);
			}
		}

		return false;
	}

	// If the player has pressed "enter" key and has inserted all the numbers,
	// check if combination is correct
	if (key == 13
		&& nInsert_ == nTot_)
	{
		numbers_[0] = string_[0] - 48;
		numbers_[1] = string_[2] - 48;
		numbers_[2] = string_[4] - 48;
		numbers_[3] = string_[6] - 48;

		// If "callback_()" returns true, then the combination is correct...
		if (callback_(numbers_))
		{
			AudioSystem::getInstance().playSound(pSuccessSound_, 0.5f);

			return true;
		}
		// ...else if "callback_()" returns false, then the combination is wrong
		else
		{
			pCombinationText_->color_ = Color(1.0f, 0.0f, 0.0f);

			AudioSystem::getInstance().playSound(pFailSound_, 0.5f);

			return false;
		}
	}

	// If the player has pressed a number key, insert it,
	// unless all the numbers are already been inserted
	if (key <= 57 && key >= 48
		&& nInsert_ != nTot_)
	{
		string_[nInsert_ * 2] = key;

		nInsert_++;

		pCombinationText_->setString(string_, Text2D::Alignment::LEFT);

		if (nInsert_ == 1)
		{
			keypadScene.add(pUndoCmdText_);
		}

		if (nInsert_ == nTot_)
		{
			keypadScene.add(pConfirmCmdText_);
		}
	}

	return false;
}

void Keypad::open(int nTot, std::function<bool(const int*)> callback)
{
	nInsert_ = 0;
	nTot_ = nTot;
	elapsedTime_ = 0.0f;
	callback_ = callback;

	for (int i = 0; i < nTot; i++)
	{
		string_[i * 2] = '_';
		string_[i * 2 + 1] = ':';
	}
	string_[nTot * 2 - 1] = '\0';

	pCombinationText_->setString(string_, Text2D::Alignment::LEFT);
	pCombinationText_->color_ = Color(1.0f, 1.0f, 1.0f);

	Scene& keypadScene = Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE];
	keypadScene.remove(pUndoCmdText_);
	keypadScene.remove(pConfirmCmdText_);
}
