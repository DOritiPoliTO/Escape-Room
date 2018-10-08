#include "game.h"
#include "glut.h"
#include "graphics/bullet_debug_drawer.h"

int windowedModeWidth;
int windowedModeHeight;
int windowWidth;
int windowHeight;
int startTime(0);
bool runFullscreen;

// @DEBUG
BulletDebugDrawer* pBulletDebugDrawer(nullptr);
bool debug_isMouseEnabled(true);
bool debug_renderPhysicsWorld(false);

void idle()
{
	int timeFromGlutInit = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)(timeFromGlutInit - startTime) * 0.001f;
	startTime = timeFromGlutInit;

	// @DEBUG
	if (debug_isMouseEnabled)
	{
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}

	AudioSystem::getInstance().update();

	if (Game::getInstance().update(deltaTime))
	{
		windowWidth = windowedModeWidth;
		windowHeight = windowedModeHeight;
		Vector2i windowPosition;
		windowPosition.x = (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2;
		windowPosition.y = (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2;
		glutPositionWindow(windowPosition.x, windowPosition.y);
		glutReshapeWindow(windowWidth, windowHeight);

		if (pBulletDebugDrawer != nullptr)
		{
			delete pBulletDebugDrawer;
			pBulletDebugDrawer = nullptr;
		}

		exit(0);
	}

	InputManager::getInstance().keyboard_.clear();

	glutPostRedisplay();
}

void display()
{
	Renderer& renderer = Renderer::getInstance();
	Game& game = Game::getInstance();

	renderer.renderScene(game.getActiveSceneId());

	if (debug_renderPhysicsWorld)
	{
		Scene& worldScene = renderer.scenes_[SceneId::WORLD_SCENE];
		if (game.getActiveSceneId() == SceneId::WORLD_SCENE)
		{
			Camera& camera = worldScene.camera_;

			pBulletDebugDrawer->render(camera.position_, camera.direction_);
		}
	}

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	Renderer::getInstance().createProjectionMatrices(width, height);

	glutWarpPointer(width / 2, height / 2);

	InputManager::getInstance().mouse_.updateDelta(Vector2i(0, 0));

	pBulletDebugDrawer->setWindowSize(width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	InputManager::getInstance().keyboard_.setKeyState(key, true);
}

void keyboardUp(unsigned char key, int x, int y)
{
	InputManager::getInstance().keyboard_.setKeyState(key, false);
}

void special(int key, int x, int y)
{
	InputManager::getInstance().keyboard_.setKeyState(key, true);

	switch (key)
	{
	case GLUT_KEY_F1:
		runFullscreen = !runFullscreen;
		if (runFullscreen)
		{
			glutFullScreen();
		}
		else
		{
			windowWidth = windowedModeWidth;
			windowHeight = windowedModeHeight;
			Vector2i windowPosition;
			windowPosition.x = (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2;
			windowPosition.y = (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2;
			glutPositionWindow(windowPosition.x, windowPosition.y);
			glutReshapeWindow(windowWidth, windowHeight);
		}
		break;
	case GLUT_KEY_F2:
		// @DEBUG
		debug_isMouseEnabled = !debug_isMouseEnabled;
		InputManager::getInstance().mouse_.updateDelta(Vector2i(0, 0));
		InputManager::getInstance().mouse_.updateLb(false);
		InputManager::getInstance().mouse_.updateRb(false);
		glutSetCursor(debug_isMouseEnabled ? GLUT_CURSOR_NONE : GLUT_CURSOR_INHERIT);
		break;
	case GLUT_KEY_F3:
		// @DEBUG
		debug_renderPhysicsWorld = !debug_renderPhysicsWorld;
		break;
	default:
		break;
	}
}

void specialUp(int key, int x, int y)
{
	InputManager::getInstance().keyboard_.setKeyState(key, false);
}

void mouseButton(int button, int state, int x, int y)
{
	// @DEBUG
	if (!debug_isMouseEnabled)
	{
		return;
	}

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			InputManager::getInstance().mouse_.updateLb(true);
		}
		else
		{
			InputManager::getInstance().mouse_.updateLb(false);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			InputManager::getInstance().mouse_.updateRb(true);
		}
		else
		{
			InputManager::getInstance().mouse_.updateRb(false);
		}
		break;
	default:
		break;
	}
}

void mouseMove(int x, int y)
{
	// @DEBUG
	if (!debug_isMouseEnabled)
	{
		return;
	}

	InputManager::getInstance().mouse_.updateDelta(Vector2i(x - windowWidth / 2, y - windowHeight / 2));
}

int main(int argc, char** argv)
{
	try
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA /*| GLUT_MULTISAMPLE*/);

		Configurator& configurator = Configurator::getInstance();
		windowedModeWidth = std::atoi(configurator.getParameterValue("window_width"));
		windowedModeHeight = std::atoi(configurator.getParameterValue("window_height"));
		runFullscreen = strcmp(configurator.getParameterValue("start_fullscreen"), "True") == 0;

		if (runFullscreen)
		{
			glutCreateWindow("Escape Room");
			glutFullScreen();
			windowWidth = glutGet(GLUT_WINDOW_WIDTH);
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		}
		else
		{
			windowWidth = windowedModeWidth;
			windowHeight = windowedModeHeight;
			Vector2i windowPosition;
			windowPosition.x = (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2;
			windowPosition.y = (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2;
			glutInitWindowPosition(windowPosition.x, windowPosition.y);
			glutInitWindowSize(windowWidth, windowHeight);
			glutCreateWindow("Escape Room");
		}

		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(special);
		glutSpecialUpFunc(specialUp);
		glutMouseFunc(mouseButton);
		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(idle);

		glutWarpPointer(windowWidth / 2, windowHeight / 2);
		glutSetCursor(GLUT_CURSOR_NONE);

		glewInit();

		BatchAllocator& batchAllocator = BatchAllocator::getInstance();
		batchAllocator.setNumberOfBlocksPerBatch<Font>(4);
		batchAllocator.setNumberOfBlocksPerBatch<Billboard3D>(4);
		batchAllocator.setNumberOfBlocksPerBatch<Billboard2D>(16);
		batchAllocator.setNumberOfBlocksPerBatch<ShadedModel3D>(32);
		batchAllocator.setNumberOfBlocksPerBatch<ShadedModel3D::Data>(32);
		batchAllocator.setNumberOfBlocksPerBatch<ShadelessModel3D>(8);
		batchAllocator.setNumberOfBlocksPerBatch<Text2D>(32);
		batchAllocator.setNumberOfBlocksPerBatch<Sound>(16);
		batchAllocator.setNumberOfBlocksPerBatch<Body>(16);

		Game::getInstance();

		pBulletDebugDrawer = new BulletDebugDrawer(
			PhysicsEngine::getInstance(),
			std::atof(configurator.getParameterValue("fov"))
		);

		startTime = glutGet(GLUT_ELAPSED_TIME);

		glutMainLoop();
	}
	catch (std::exception exception)
	{
		std::cout << exception.what() << std::endl;

		if (pBulletDebugDrawer != nullptr)
		{
			delete pBulletDebugDrawer;
			pBulletDebugDrawer = nullptr;
		}

		exit(-1);
	}
}
