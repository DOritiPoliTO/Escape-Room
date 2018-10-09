#include "game.h"

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}

Game::Game() :
	keypad_(),
	diary_(),
	world_(),
	state_(Game::State::START),
	elapsedTime_(0.0f),
	fpsCounter_(0.5f),
	pVideo_(nullptr),
	pVideoFrameBillboard_(nullptr),
	pRttBillboard_(nullptr),
	pStartScreenBillboard_(nullptr),
	pFpsText_(nullptr),
	pExitText_(nullptr),
	pNewEntryInDiaryNotifyText_(nullptr),
	pEndGameText_(nullptr),
	pCreditsText_(nullptr),
	pAmbientMusic_(nullptr),
	newNoteInDiaryFlag_(false),
	activeSceneId_(SceneId::GAME_START_SCENE),
	seconds_(0.0f),
	minutes_(0),
	hours_(0)
{
	try
	{
		// Initialise components and create resources
		{
			pRttBillboard_ = new Billboard2D(1920, 1080, 1.0f, 1080.0f / 1920.0f, false, false);

			pVideoFrameBillboard_ = new Billboard2D(1920, 1080, 0.5f, 0.5f * 1080.0f / 1920.0f,
				true, false);

			pStartScreenBillboard_ = new Billboard2D("../EscapeRoom/data/textures/start_screen.png", 1.0f);

			pFpsText_ = new Text2D("../EscapeRoom/data/fonts/candara.fnt");

			pExitText_ = new Text2D(
				"Press Esc again to exit,\nor press another key to return to game",
				Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pNewEntryInDiaryNotifyText_ = new Text2D("A new note has been added to the diary",
				Text2D::Alignment::CENTER, "../EscapeRoom/data/fonts/candara.fnt");

			pEndGameText_ = new Text2D("../EscapeRoom/data/fonts/candara.fnt");

			pCreditsText_ = new Text2D("Damiano Oriti (Programming, Modeling and Animation)\nSara Battelini (Modeling and Animation)",
				Text2D::Alignment::CENTER, "../EscapeRoom/data/fonts/candara.fnt");

			pThanksText_ = new Text2D("Special Thanks go to Antonio Marceddu for the Music of the Game",
				Text2D::Alignment::CENTER, "../EscapeRoom/data/fonts/candara.fnt");

			pAmbientMusic_ = AudioSystem::getInstance().loadSound(
				"../EscapeRoom/data/sounds/music_loop.mp3",
				true
			);
		}

		// Configure components and resources
		{
			Scene* scenes = Renderer::getInstance().scenes_;

			scenes[SceneId::VIDEO_SCENE].add(pRttBillboard_);
			scenes[SceneId::KEYPAD_SCENE].add(pRttBillboard_);

			scenes[SceneId::VIDEO_SCENE].add(pVideoFrameBillboard_);

			pStartScreenBillboard_->colorScale_.a = 0.0f;
			scenes[SceneId::GAME_START_SCENE].add(pStartScreenBillboard_);

			pFpsText_->position_ = Vector2f(0.98f, 0.98f);
			pFpsText_->scale_ = 0.8f;
			pFpsText_->color_ = Color(1.0f, 0.0f, 0.0f);
			pFpsText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pFpsText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			scenes[SceneId::WORLD_SCENE].add(pFpsText_);
			scenes[SceneId::DIARY_SCENE].add(pFpsText_);
			scenes[SceneId::EXIT_MENU_SCENE].add(pFpsText_);
			scenes[SceneId::VIDEO_SCENE].add(pFpsText_);
			scenes[SceneId::KEYPAD_SCENE].add(pFpsText_);
			scenes[SceneId::GAME_START_SCENE].add(pFpsText_);
			scenes[SceneId::GAME_END_SCENE].add(pFpsText_);
			scenes[SceneId::CREDITS_SCENE].add(pFpsText_);

			pExitText_->position_ = Vector2f(0.0f, 0.0f);
			pExitText_->color_ = Color(1.0f, 1.0f, 1.0f);
			scenes[SceneId::EXIT_MENU_SCENE].add(pExitText_);

			pNewEntryInDiaryNotifyText_->position_ = Vector2f(0.0f, 0.9f);
			pNewEntryInDiaryNotifyText_->color_ = Color(1.0f, 1.0f, 0.0f);
			pNewEntryInDiaryNotifyText_->effectType_ = Text2D::EffectType::GLOW;
			pNewEntryInDiaryNotifyText_->effectColor_ = Color(0.0f, 1.0f, 0.0f);

			pEndGameText_->position_ = Vector2f(0.0f, -0.2f);
			pEndGameText_->color_ = Color(1.0f, 1.0f, 1.0f);
			pEndGameText_->scale_ = 1.2f;
			pEndGameText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pEndGameText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			scenes[SceneId::GAME_END_SCENE].add(pEndGameText_);

			pCreditsText_->position_ = Vector2f(0.0f, -0.1f);
			pCreditsText_->color_ = Color(1.0f, 1.0f, 1.0f);
			pCreditsText_->scale_ = 1.4f;
			pCreditsText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pCreditsText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			scenes[SceneId::CREDITS_SCENE].add(pCreditsText_);

			pThanksText_->position_ = Vector2f(0.0f, -0.6f);
			pThanksText_->color_ = Color(1.0f, 1.0f, 1.0f);
			pThanksText_->scale_ = 0.8f;
			pThanksText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pThanksText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			scenes[SceneId::CREDITS_SCENE].add(pThanksText_);
		}
	}
	catch (...)
	{
		if (pAmbientMusic_ != nullptr)
		{
			delete pAmbientMusic_;
			pAmbientMusic_ = nullptr;
		}

		if (pThanksText_ != nullptr)
		{
			delete pThanksText_;
			pThanksText_ = nullptr;
		}

		if (pCreditsText_ != nullptr)
		{
			delete pCreditsText_;
			pCreditsText_ = nullptr;
		}

		if (pEndGameText_ != nullptr)
		{
			delete pEndGameText_;
			pEndGameText_ = nullptr;
		}

		if (pNewEntryInDiaryNotifyText_ != nullptr)
		{
			delete pNewEntryInDiaryNotifyText_;
			pNewEntryInDiaryNotifyText_ = nullptr;
		}

		if (pExitText_ != nullptr)
		{
			delete pExitText_;
			pExitText_ = nullptr;
		}

		if (pFpsText_ != nullptr)
		{
			delete pFpsText_;
			pFpsText_ = nullptr;
		}

		if (pStartScreenBillboard_ != nullptr)
		{
			delete pStartScreenBillboard_;
			pStartScreenBillboard_ = nullptr;
		}

		if (pVideoFrameBillboard_ != nullptr)
		{
			delete pVideoFrameBillboard_;
			pVideoFrameBillboard_ = nullptr;
		}

		if (pRttBillboard_ != nullptr)
		{
			delete pRttBillboard_;
			pRttBillboard_ = nullptr;
		}

		if (pVideo_ != nullptr)
		{
			delete pVideo_;
			pVideo_ = nullptr;
		}

		throw;
	}
}

Game::~Game()
{
	if (pAmbientMusic_ != nullptr)
	{
		delete pAmbientMusic_;
		pAmbientMusic_ = nullptr;
	}

	if (pThanksText_ != nullptr)
	{
		delete pThanksText_;
		pThanksText_ = nullptr;
	}

	if (pCreditsText_ != nullptr)
	{
		delete pCreditsText_;
		pCreditsText_ = nullptr;
	}

	if (pEndGameText_ != nullptr)
	{
		delete pEndGameText_;
		pEndGameText_ = nullptr;
	}

	if (pNewEntryInDiaryNotifyText_ != nullptr)
	{
		delete pNewEntryInDiaryNotifyText_;
		pNewEntryInDiaryNotifyText_ = nullptr;
	}

	if (pExitText_ != nullptr)
	{
		delete pExitText_;
		pExitText_ = nullptr;
	}

	if (pFpsText_ != nullptr)
	{
		delete pFpsText_;
		pFpsText_ = nullptr;
	}

	if (pStartScreenBillboard_ != nullptr)
	{
		delete pStartScreenBillboard_;
		pStartScreenBillboard_ = nullptr;
	}

	if (pVideoFrameBillboard_ != nullptr)
	{
		delete pVideoFrameBillboard_;
		pVideoFrameBillboard_ = nullptr;
	}

	if (pRttBillboard_ != nullptr)
	{
		delete pRttBillboard_;
		pRttBillboard_ = nullptr;
	}

	if (pVideo_ != nullptr)
	{
		delete pVideo_;
		pVideo_ = nullptr;
	}
}

bool Game::update(float deltaTime)
{
	// Update fps (frames per second) counter and related 2d text
	{
		fpsCounter_.update(deltaTime);
		pFpsText_->setString(fpsCounter_.getFps(), "fps: %.1f", Text2D::Alignment::RIGHT);
	}

	// Update play time
	updatePlayTime(deltaTime);

	bool shouldExitGame = false;
	switch (state_)
	{
	case Game::State::IN_WORLD:
		updateGameWorld(deltaTime);
		break;
	case Game::State::USING_KEYPAD:
		handleKeypad(deltaTime);
		break;
	case Game::State::PLAYING_VIDEO:
		advanceVideo(deltaTime);
		break;
	case Game::State::READING_DIARY:
		handleDiary(deltaTime);
		break;
	case Game::State::IN_EXIT_MENU:
		shouldExitGame = handleExitMenu();
		break;
	case Game::State::START:
		handleStart(deltaTime);
		break;
	case Game::State::END:
		handleEnd();
		break;
	case Game::State::CREDITS:
		shouldExitGame = handleCredits(deltaTime);
		break;
	default:
		break;
	}

	return shouldExitGame;
}

void Game::playVideo(const char* videoFilename, std::function<void(void)> callback)
{
	pVideo_ = new Video(
		videoFilename,
		std::bind(
			static_cast<void(Texture::*)(const void*)>(&Texture::write),
			pVideoFrameBillboard_->getTexture(),
			std::placeholders::_1
		)
	);
	pVideoFrameBillboard_->colorScale_.a = 0.0f;

	Renderer::getInstance().renderSceneToTexture(
		SceneId::WORLD_SCENE,
		pRttBillboard_->getTexture()
	);

	Color clearColor(0.0f, 0.0f, 0.0f);
	pVideoFrameBillboard_->getTexture()->clear(clearColor);

	videoCallback_ = callback;

	elapsedTime_ = 0.0f;

	activeSceneId_ = SceneId::VIDEO_SCENE;
	state_ = Game::State::PLAYING_VIDEO;
}

void Game::useKeypad(int nTot, std::function<bool(const int*)> callback)
{
	keypad_.open(nTot, callback);

	Renderer::getInstance().renderSceneToTexture(
		SceneId::WORLD_SCENE,
		pRttBillboard_->getTexture()
	);

	activeSceneId_ = SceneId::KEYPAD_SCENE;
	state_ = Game::State::USING_KEYPAD;
}

void Game::addEntryInDiary(Diary::Entry* pEntry)
{
	elapsedTime_ = 0.0f;
	newNoteInDiaryFlag_ = true;

	Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].add(pNewEntryInDiaryNotifyText_);

	diary_.addEntry(pEntry);
}

void Game::end()
{
	char string[128];
	sprintf(string, "Congratulations! You Have Beated the Game in %02d:%02d:%02d\n\nPress Enter to Continue",
		hours_, minutes_, (int)seconds_);
	pEndGameText_->setString(string, Text2D::Alignment::CENTER);

	activeSceneId_ = SceneId::GAME_END_SCENE;
	state_ = Game::State::END;
}

int Game::getActiveSceneId() const
{
	return activeSceneId_;
}

inline void Game::advanceVideo(float deltaTime)
{
	bool isPlaying = pVideo_->processNextFrame(deltaTime);

	elapsedTime_ += deltaTime;
	if (elapsedTime_ <= 1.0f)
	{
		pVideoFrameBillboard_->colorScale_.a = smoothStart2(elapsedTime_);
	}

	if (!isPlaying)
	{
		delete pVideo_;
		pVideo_ = nullptr;

		if (videoCallback_ != nullptr)
		{
			videoCallback_();
		}

		activeSceneId_ = SceneId::WORLD_SCENE;

		state_ = Game::State::IN_WORLD;
	}
}

inline bool Game::handleExitMenu()
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 27)
	{
		return true;
	}
	else if (key != -1)
	{
		activeSceneId_ = SceneId::WORLD_SCENE;

		state_ = Game::State::IN_WORLD;

		return false;
	}
	else
	{
		return false;
	}
}

inline void Game::handleDiary(float deltaTime)
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 'q'
		|| key == 27)
	{
		activeSceneId_ = SceneId::WORLD_SCENE;

		state_ = Game::State::IN_WORLD;

		return;
	}

	diary_.update(deltaTime);
}

inline void Game::handleKeypad(float deltaTime)
{
	if (keypad_.update(deltaTime))
	{
		activeSceneId_ = SceneId::WORLD_SCENE;

		state_ = Game::State::IN_WORLD;
	}
}

inline void Game::updateGameWorld(float deltaTime)
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 'q')
	{
		Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].remove(pNewEntryInDiaryNotifyText_);

		diary_.open();

		activeSceneId_ = SceneId::DIARY_SCENE;

		state_ = Game::State::READING_DIARY;

		return;
	}
	else if (key == 27)
	{
		activeSceneId_ = SceneId::EXIT_MENU_SCENE;

		state_ = Game::State::IN_EXIT_MENU;

		return;
	}

	if (newNoteInDiaryFlag_)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ > 4.0f)
		{
			Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].remove(pNewEntryInDiaryNotifyText_);
			newNoteInDiaryFlag_ = false;
		}
	}

	world_.update(deltaTime);
}

inline void Game::handleStart(float deltaTime)
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 27
		|| key == 13)
	{
		AudioSystem::getInstance().playSound(pAmbientMusic_, 0.25f);
		activeSceneId_ = SceneId::WORLD_SCENE;
		state_ = Game::State::IN_WORLD;
	}

	elapsedTime_ += deltaTime;
	if (elapsedTime_ <= 2.0f)
	{
		pStartScreenBillboard_->colorScale_.a = smoothStart2(elapsedTime_ / 2.0f);
		return;
	}

	if (elapsedTime_ > 4.0f)
	{
		AudioSystem::getInstance().playSound(pAmbientMusic_, 0.25f);
		activeSceneId_ = SceneId::WORLD_SCENE;
		state_ = Game::State::IN_WORLD;
	}
}

inline void Game::handleEnd()
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 13)
	{
		elapsedTime_ = 0.0f;
		activeSceneId_ = SceneId::CREDITS_SCENE;
		state_ = Game::State::CREDITS;
	}
}

inline bool Game::handleCredits(float deltaTime)
{
	int key = InputManager::getInstance().keyboard_.getLastKeyPressed();
	if (key == 27
		|| key == 13)
	{
		return true;
	}

	elapsedTime_ += deltaTime;
	if (elapsedTime_ >= 4.0f)
	{
		return true;
	}

	return false;
}

inline void Game::updatePlayTime(float deltaTime)
{
	seconds_ += deltaTime;
	if (seconds_ > 60.0f)
	{
		seconds_ -= 60.0f;

		minutes_++;
		if (minutes_ == 60)
		{
			minutes_ = 0;

			hours_++;
		}
	}
}
