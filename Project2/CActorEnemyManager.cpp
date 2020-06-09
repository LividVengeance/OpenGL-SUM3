#include "CActorEnemyManager.h"

CActorEnemyManager::CActorEnemyManager(CCamera* _gameCamera, CObject* object)
{
	gameCamera = _gameCamera;

	currentWave = 1;

	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/Blinn-Phong.fs");

	const char* fileLocationPlay = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocationPlay, &actorTex);

	actorSphere = new CSphere();

	AddEnemy(object);
}

CActorEnemyManager::~CActorEnemyManager()
{
}

void CActorEnemyManager::Update(GLfloat* deltaTime, CObject* object)
{
	for (int i = 0; i < enemysInScene.size(); i++)
	{
		enemysInScene[i]->Update();
		enemysInScene[i]->SteeringSeek(*deltaTime, object);
	}

	WaveCheck(deltaTime, object);
}

void CActorEnemyManager::Render()
{
	for (int i = 0; i < enemysInScene.size(); i++)
	{
		enemysInScene[i]->Render();
	}
}

void CActorEnemyManager::AddEnemy(CObject* object)
{
	std::cout << "Create new Actor" << std::endl;
	actorEnemy = new CActorEnemy(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex);
	
	int randX = rand() % 40;
	int randZ = rand() % 40;
	int dist = 10; // Min dist from player that enemy can spawn

	randX, randZ += dist;
	actorEnemy->objPosition.x = object->objPosition.x + randX;
	actorEnemy->objPosition.z = object->objPosition.z + randZ;

	enemysInScene.push_back(actorEnemy);

	std::cout << enemysInScene.size() << std::endl;
}

void CActorEnemyManager::WaveCheck(GLfloat* deltaTime, CObject* object)
{
	elapsedTime += *deltaTime;

	// Current wave check
	if (elapsedTime >= (timeScalePerWave * currentWave))
	{
		currentWave += 1;
	}

	// Enemys in scene check
	while ((enemysInScene.size() * currentWave) < currentWave * actorsPerWave)
	{
		std::cout << enemysInScene.size() << std::endl;
		AddEnemy(object);
	}
}

void CActorEnemyManager::TextureGen(const char* textureLocation, GLuint* texture)
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