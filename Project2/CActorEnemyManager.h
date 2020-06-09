#pragma once
#include "CShaderLoader.h"
#include "CCamera.h"

#include "CActorEnemy.h"

#include "CSphere.h"
#include "Model.h"

#include <vector>

class CActorEnemyManager
{
public:
	CActorEnemyManager(CCamera* _gameCamera, CObject* object);
	~CActorEnemyManager();

	void Update(GLfloat* deltaTime, CObject* object);
	void Render();

	std::vector<CActorEnemy*> enemysInScene;

	int currentWave;

private:
	void AddEnemy(CObject* object);
	void WaveCheck(GLfloat* deltaTime, CObject* object);
	void TextureGen(const char* textureLocation, GLuint* texture);

	int actorsPerWave = 10;
	int timeScalePerWave = 10;

	// Textures
	GLuint actorTex;

	// Actor
	CActorEnemy* actorEnemy;

	// Meshes
	CSphere* actorSphere;
	Model* model;

	// Program
	GLint program;

	// Utilites
	CCamera* gameCamera;
	GLfloat elapsedTime;
};

