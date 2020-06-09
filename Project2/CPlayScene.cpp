#include "CPlayScene.h"

#include <math.h>

CPlayScene::CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem)
{
	gameCamera = _gameCamera;
	gameInput = _gameInput;
	audioSystem = _audioSystem;

	gameSceneScore = 0;

	// Creates Programs For Objects In Scene
	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/Blinn-Phong.fs");

	skyboxProgram = CShaderLoader::CreateProgram("Resources/Shaders/skybox.vs",
		"Resources/Shaders/skybox.fs");

	enemyProgram = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/RimLighting.fs");

	pickupProgram = CShaderLoader::CreateProgram("Resources/Shaders/Reflection.vs",
		"Resources/Shaders/Reflection.fs");

	// Generate Texture For Actors In Scene
	const char* fileLocationPlay = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocationPlay, &actorTex);
	const char* fileLocationPickup = "Resources/Textures/Actors/healthPickupSprite.png";
	TextureGen(fileLocationPickup, &pickupTex);

	// Creates Meshes/Models
	actorSphere = new CSphere();
	actorEnemyPyramid = new CPyramid();
	model = new Model("Resources/Models/Tank/Tank.obj", gameCamera);

	// Create Skybox
	gameSkybox = new CSkybox(&skyboxProgram, gameCamera);
	
	// Create Actors
	gameActor = new CActor(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex, audioSystem);
	actorEnemy = new CActorEnemy(&enemyProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex);
	actorPickup = new CActorPickupScore(&pickupProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex);
	actorHealthPickup = new CActorPickupHealth(&enemyProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &pickupTex);

	// Labels
	actorScoreLabel = new CTextLabel("Score: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 520.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	actorHealthLabel = new CTextLabel("Health: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 560.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);

	// Move Objects Start Position
	actorEnemy->objPosition.x = 20;
	actorPickup->objPosition.x = -20;
}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Render()
{
	glUseProgram(program);

	gameSkybox->Render();

	// Actors
	gameActor->Render();
	gameActor->BulletRender(); // Renders all the bullets in the scene
	actorEnemy->Render();
	actorPickup->RenderReflection(gameSkybox);
	actorHealthPickup->Render();
	//model->Render(actorEnemy);

	// Labels
	actorHealthLabel->Render();
	actorScoreLabel->Render();

	glBindVertexArray(0);		// Unbinding VAO
	glUseProgram(0);
}

void CPlayScene::Update(GLfloat* deltaTime, ESceneManager* _currentScene)
{
	currentScene = _currentScene;
	gameCamera->Update(*deltaTime);
	gameSkybox->Update();

	// Actors
	gameActor->Update();
	gameActor->MoveInput(*deltaTime, gameInput);
	gameActor->ShootInput(*deltaTime, gameInput); // Creates bullet on mouse click
	gameActor->BulletUpdate(); // Updates all the bullents in scene
	actorEnemy->Update();
	actorEnemy->SteeringSeek(*deltaTime, gameActor);
	actorPickup->Update();
	actorHealthPickup->Update();

	// gameActor lose health on collision actorEnemy
	if (CollisionCheck(gameActor, actorEnemy))
	{
		gameActor->actorHealth--;
	}
	// gameActor gain score on collision actorPickup
	if (CollisionCheck(gameActor, actorPickup))
	{
		gameActor->actorScore += 100;
	}
	// gameActor gain health on collision actorHealthPickup
	if (CollisionCheck(gameActor, actorHealthPickup))
	{
		gameActor->actorHealth += 5;
	}
	// Destory actorEnemy on collision actorBullet
	std::map<CActorBullet*, vec2>::iterator bulletIndex = gameActor->bulletsInScene.begin();
	for (int i = 0; i < gameActor->bulletsInScene.size(); i++)
	{
		if (CollisionCheck(bulletIndex->first, actorEnemy))
		{
			delete bulletIndex->first;
			bulletIndex = gameActor->bulletsInScene.erase(bulletIndex);
			std::cout << "Bullet Hit Enemy" << std::endl;
		}
		else
		{
			bulletIndex++;
		}
	}

	// Updates the score label
	std::string scoreStr = "Score: ";
	scoreStr += std::to_string(gameActor->actorScore);
	actorScoreLabel->SetText(scoreStr);

	// Updates the health label
	std::string healthStr = "Health: ";
	healthStr += std::to_string(gameActor->actorHealth);
	actorHealthLabel->SetText(healthStr);

	// Change Scene To GameOverScene On player Death
	if (gameActor->actorHealth <= 0)
	{
		*currentScene = EGameOverScene;
		gameSceneScore = gameActor->actorScore;
		ResetScene(); // Resets the scene for the next play
	}

	CollisionCheck(gameActor, actorEnemy);
}

void CPlayScene::TextureGen(const char* textureLocation, GLuint* texture)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	int width, height;
	unsigned char* image1 = SOIL_load_image(textureLocation, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CPlayScene::ResetScene()
{
	delete gameActor;
	gameActor = new CActor(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex, audioSystem);
	gameActor->actorHealth = 10;
}

bool CPlayScene::CollisionCheck(CObject* actorOne, CObject* actorTwo)
{
	if (glm::length(actorOne->objPosition - actorTwo->objPosition) < 1)
	{
		return(true);
	}
	return(false);
}