#pragma once
#include "CGameManager.h"

CGameManager* globalPointerGM;

void CGameManager::CreateAudioSystem()
{
	/// Creates the audio system
	FMOD_RESULT result;
	result = System_Create(&audioSystem);
	if (result != FMOD_OK)
	{
		std::cout << "Failed to create audioSystem" << std::endl;
	}
	result = audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
	{
		std::cout << "Failed to initialize the audioSystem" << std::endl;
	}
}

CGameManager::CGameManager(int argc, char** argv)
{
	globalPointerGM = this;

	// Setup and create at glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(Utils::SCR_WIDTH, Utils::SCR_HEIGHT);
	glutCreateWindow("OpenGL Exercises - GD1P04");

	// Sets up all GL function callbacks based on pc hardware
	if (glewInit() != GLEW_OK)
	{
		// If glew fails to setup will throw error message
		std::cout << "Glew Faild to initialize. Now aborting." << std::endl;
		system("pause");
	}

	// Sets the clear colour
	glClearColor(23.0f / 256.0f, 27.0f / 256.0f, 27.0f / 256.0f, 1.0f);

	// Enabling Culling
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	// Enabling Culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// Depth testing/function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create Camera
	gameCamera = new CCamera();

	// Create Input Controller
	gameInput = new CInput();

	// Create Audio Syetem																									  
	CreateAudioSystem();

	// Creates and plays the background music
	CAudio backingTrack("Resources/Audio/Background.mp3", audioSystem, true);
	backingTrack.PlaySound();

/// GAME SCENES
	// Creates the main menu scene
	gameMainMenuScene = new CMainMenuScene(gameCamera, gameInput);

	// Creates the play scene
	gamePlayScene = new CPlayScene(gameCamera, gameInput, audioSystem);

	// Creates the game over scene
	gameOverScene = new CGameOverScene(gameCamera, gameInput, gamePlayScene->GetPlayerScore());

	currentScene = EMainMenuScene;
}

CGameManager::~CGameManager()
{
}

void CGameManager::Render()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Renders based on current scene
	if (currentScene == EMainMenuScene)
	{
		gameMainMenuScene->Render();
	}
	else if (currentScene == EPlayScene)
	{
		gamePlayScene->Render();
	}
	else if (currentScene == EGameOverScene)
	{
		gameOverScene->Render();
	}

	glutSwapBuffers();
}

void CGameManager::Update()
{
	// Update information
	currentTime = glutGet(GLUT_ELAPSED_TIME);	// Get current time
	currentTime = currentTime * 0.001f;			// Converting to time seconds (From miliseconds)
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	// Updates based on current scene
	if (currentScene == EMainMenuScene)
	{
		gameMainMenuScene->Update(&deltaTime, &currentScene);
	}
	else if (currentScene == EPlayScene)
	{
		gamePlayScene->Update(&deltaTime, &currentScene);
	}
	else if (currentScene == EGameOverScene)
	{
		gameOverScene->Update(&deltaTime, &currentScene);
	}

	glutPostRedisplay();
}

void CGameManager::KeyBoardDown(unsigned char key, int x, int y)
{
	gameInput->KeyboardDown(key, x, y);
}

void CGameManager::KeyBoardUp(unsigned char key, int x, int y)
{
	gameInput->KeyboardUp(key, x, y);
}

void CGameManager::MousePassiveMove(int x, int y)
{
	gameInput->MousePassiveMove(x, y);
}

void CGameManager::MouseClick(int button, int state, int x, int y)
{
	gameInput->MouseClick(button, state, x, y);
}

void CGameManager::MouseMove(int x, int y)
{
	gameInput->MouseMove(x, y);
}

void KeyboardDownRedirect(unsigned char key, int x, int y)
{
	globalPointerGM->KeyBoardDown(key, x, y);
}

void KeyboardUpRedirect(unsigned char key, int x, int y)
{
	globalPointerGM->KeyBoardUp(key, x, y);
}

void MousePassiveMoveRedirect(int x, int y)
{
	globalPointerGM->MousePassiveMove(x, y);
}

void MouseClickRedirect(int button, int state, int x, int y)
{
	globalPointerGM->MouseClick(button, state, x, y);
}

void MouseMoveRedirect(int x, int y)
{
	globalPointerGM->MouseMove(x, y);
}

void RenderRedirect()
{
	/// Allows glut to look at func in class
	globalPointerGM->Render();
}

void UpdateRedirect()
{
	/// Allows glut to look at func in class
	globalPointerGM->Update();
}

void CGameManager::ManagerMain()
{
	/// Register callbacks
	glutDisplayFunc(RenderRedirect);
	glutIdleFunc(UpdateRedirect);

	// Updating audio in audioSystem
	audioSystem->update();

	// KeyBoard Inputs
	glutKeyboardFunc(KeyboardDownRedirect);
	glutKeyboardUpFunc(KeyboardUpRedirect);

	// Mouse Inputs
	glutMouseFunc(MouseClickRedirect);
	glutMotionFunc(MouseMoveRedirect);
	glutPassiveMotionFunc(MousePassiveMoveRedirect);

	glutMainLoop();
}