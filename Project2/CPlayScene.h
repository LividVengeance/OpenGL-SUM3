#pragma once
#include "CShaderLoader.h"
#include "CCamera.h"
#include "CInput.h"
#include "CTextLabel.h"

#include "CActorEnemy.h"
#include "CActor.h"
#include "CActorPickupHealth.h"
#include "CActorPickupScore.h"

#include "CActorEnemyManager.h"

#include "CPyramid.h"
#include "CSphere.h"
#include "CSkybox.h"
#include "Model.h"

class CPlayScene
{
public:
	CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem);
	~CPlayScene();

	void Render();
	void Update(GLfloat* deltaTime, ESceneManager* _currentScene);

	int gameSceneScore;

private:
	void TextureGen(const char* textureLocation, GLuint* texture);
	bool CollisionCheck(CObject* actorOne, CObject* actorTwo);
	void ResetScene();
	void AllCollisionsInScene();

	// Textures
	GLuint actorTex;
	GLuint pickupTex;

	// Actors
	CActor* gameActor;
	CActorPickupScore* actorPickup;
	CActorPickupHealth* actorHealthPickup;

	CActorEnemyManager* enemyManager;

	// Meshes
	CSphere* actorSphere;

	// Labels
	CTextLabel* actorHealthLabel;
	CTextLabel* actorScoreLabel;
	CTextLabel* waveLabel;

	// Programs
	GLint program;
	GLint skyboxProgram;
	GLint pickupProgram;
	GLint healthProgram;

	// Audio
	FMOD::System* audioSystem;

	// Utilities
	ESceneManager* currentScene;
	CSkybox* gameSkybox;
	CCamera* gameCamera;
	CInput* gameInput;
	Model* model;
	mat4 view;

	bool scorePickupSpawn;
	bool healthPickupSpawn;
};

