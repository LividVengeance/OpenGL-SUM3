#include "CPlayScene.h"

#include <math.h>
#include <map>

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

	healthProgram = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
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
	model = new Model("Resources/Models/Tank/Tank.obj", gameCamera);

	// Create Skybox
	gameSkybox = new CSkybox(&skyboxProgram, gameCamera);
	
	// Create Actors
	gameActor = new CActor(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex, audioSystem);
	actorPickup = new CActorPickupScore(&pickupProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex);
	actorHealthPickup = new CActorPickupHealth(&healthProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &pickupTex);

	enemyManager = new CActorEnemyManager(gameCamera, gameActor);

	// Labels
	actorScoreLabel = new CTextLabel("Score: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 520.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	actorHealthLabel = new CTextLabel("Health: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 560.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	waveLabel = new CTextLabel("Wave: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 480.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);

	// Move Objects Start Position
	actorPickup->objPosition.x = -20;
	actorHealthPickup->objPosition.x = 20;

	scorePickupSpawn = true;
	healthPickupSpawn = true;

	gameActor->objScaleAmount = 0.3f;
}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Render()
{
	glUseProgram(program);

	gameSkybox->Render();

	// Actors
	model->Render(gameActor);
	gameActor->BulletRender(); // Renders all the bullets in the scene

	

	if (scorePickupSpawn)
	{
		actorPickup->RenderReflection(gameSkybox);
	}
	if (healthPickupSpawn)
	{
		actorHealthPickup->Render();
	}

	enemyManager->Render();

	// Labels
	actorHealthLabel->Render();
	actorScoreLabel->Render();
	waveLabel->Render();

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

	if (scorePickupSpawn)
	{
		actorPickup->Update();
	}
	if (healthPickupSpawn)
	{
		actorHealthPickup->Update();
	}
	
	enemyManager->Update(deltaTime, gameActor);

	AllCollisionsInScene();

	// Updates the score label
	std::string scoreStr = "Score: ";
	scoreStr += std::to_string(gameActor->actorScore);
	actorScoreLabel->SetText(scoreStr);

	// Updates the health label
	std::string healthStr = "Health: ";
	healthStr += std::to_string(gameActor->actorHealth);
	actorHealthLabel->SetText(healthStr);

	// Updates the wave label
	std::string waveStr = "Wave: ";
	waveStr += std::to_string(enemyManager->currentWave);
	waveLabel->SetText(waveStr);

	// Change Scene To GameOverScene On player Death
	if (gameActor->actorHealth <= 0)
	{
		*currentScene = EGameOverScene;
		gameSceneScore = gameActor->actorScore;
		ResetScene(); // Resets the scene for the next play
	}
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

void CPlayScene::AllCollisionsInScene()
{
	// gameActor lose health on collision actorEnemy
	for (int i = 0; i < enemyManager->enemysInScene.size(); i++)
	{
		if (CollisionCheck(gameActor, enemyManager->enemysInScene[i]))
		{
			gameActor->actorHealth--;
			delete enemyManager->enemysInScene[i]; // Deletes object
			enemyManager->enemysInScene.erase(enemyManager->enemysInScene.begin() + i); // Deletes from vector
			i--;
			gameActor->actorScore += 10;
		}
	}

	// gameActor gain score on collision actorPickup
	if (CollisionCheck(gameActor, actorPickup))
	{
		gameActor->actorScore += 100;
		scorePickupSpawn = false;
		delete actorPickup;
	}

	// gameActor gain health on collision actorHealthPickup
	if (CollisionCheck(gameActor, actorHealthPickup))
	{
		gameActor->actorHealth += 5;
		healthPickupSpawn = false;
		delete actorHealthPickup;
	}

	//Destory actorEnemy on collision actorBullet
	for (int j = 0; j < enemyManager->enemysInScene.size(); j++) // For every enemy in scene
	{
		std::map<CActorBullet*, vec2>::iterator bulletsIndex = gameActor->bulletsInScene.begin();	
		for (int i = 0; i < gameActor->bulletsInScene.size(); i++) // Checks every bullet
		{
			if (CollisionCheck(bulletsIndex->first, enemyManager->enemysInScene[j]))
			{
				// Delete the bullet
				if (bulletsIndex->first != nullptr)
				{
					delete bulletsIndex->first;
				}
				bulletsIndex = gameActor->bulletsInScene.erase(bulletsIndex);

				if (enemyManager->enemysInScene[j] != nullptr)
				{
					delete enemyManager->enemysInScene[j]; // Deletes object
				}
				enemyManager->enemysInScene.erase(enemyManager->enemysInScene.begin() +j); // Deletes from vector
				gameActor->actorScore += 10;
			}
			else
			{
				bulletsIndex++;
			}
		}
	}
}