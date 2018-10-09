#pragma once

#include <new>
#include "input/input_manager.h"
#include "graphics/renderer.h"
#include "world/world.h"
#include "interface/keypad.h"
#include "graphics/video.h"
#include "interface/diary.h"
#include "interface/fps_counter.h"
#include "audio/audio_system.h"
#include "graphics/scene.h"

enum SceneId;

class Game
{
public:
	World world_;

	static Game& getInstance();

	bool update(float deltaTime);
	void playVideo(const char* videoFilename, std::function<void(void)> callback);
	void useKeypad(int nTot, std::function<bool(const int*)> callback);
	void addEntryInDiary(Diary::Entry* pEntry);
	void end();

	int getActiveSceneId() const;

private:
	enum State;

	State state_;
	Keypad keypad_;
	Diary diary_;
	FpsCounter fpsCounter_;
	Video* pVideo_;
	std::function<void(void)> videoCallback_;
	Billboard2D* pVideoFrameBillboard_;
	Billboard2D* pRttBillboard_;
	Billboard2D* pStartScreenBillboard_;
	Text2D* pFpsText_;
	Text2D* pExitText_;
	Text2D* pNewEntryInDiaryNotifyText_;
	Text2D* pEndGameText_;
	Text2D* pCreditsText_;
	Text2D* pThanksText_;
	Sound* pAmbientMusic_;
	float elapsedTime_;
	bool newNoteInDiaryFlag_;
	SceneId activeSceneId_;
	float seconds_;
	int minutes_;
	int hours_;

	Game();
	Game(const Game& other);
	~Game();

	inline void advanceVideo(float deltaTime);
	inline bool handleExitMenu();
	inline void handleDiary(float deltaTime);
	inline void handleKeypad(float deltaTime);
	inline void updateGameWorld(float deltaTime);
	inline void handleStart(float deltaTime);
	inline void handleEnd();
	inline bool handleCredits(float deltaTime);
	inline void updatePlayTime(float deltaTime);
};

enum Game::State
{
	IN_WORLD,
	USING_KEYPAD,
	PLAYING_VIDEO,
	READING_DIARY,
	IN_EXIT_MENU,
	START,
	END,
	CREDITS
};

enum SceneId
{
	WORLD_SCENE = 0,
	DIARY_SCENE = 1,
	EXIT_MENU_SCENE = 2,
	VIDEO_SCENE = 3,
	KEYPAD_SCENE = 4,
	GAME_START_SCENE = 5,
	GAME_END_SCENE = 6,
	CREDITS_SCENE = 7
};
