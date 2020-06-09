#pragma once

#include "CShaderLoader.h"
#include "CCamera.h"
#include "CInput.h"
#include "CTextLabel.h"

#include "CActorEnemy.h"
#include "CActor.h"
#include "CActorPickupHealth.h"
#include "CPyramid.h"
#include "CSphere.h"
#include "CSkybox.h"
#include "CActorPickupScore.h"

#include "Model.h"

class CPlayScene
{
public:
	CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem);
	~CPlayScene();

	void Render();
	void Update(GLfloat* deltaTime, ESceneManager* _currentScene);

	void TextureGen(const char* textureLocation, GLuint* texture);
	bool CollisionCheck(CObject* actorOne, CObject* actorTwo);
	void ResetScene();
	int GetPlayerScore();


	ESceneManager* currentScene;

	// Textures
	GLuint actorTex;
	GLuint pickupTex;

	// Actors
	CActor* gameActor;
	CActorEnemy* actorEnemy;
	CActorPickupScore* actorPickup;
	CActorPickupHealth* actorHealthPickup;

	// Meshes
	CPyramid* actorEnemyPyramid;
	CSphere* actorSphere;

	// Labels
	CTextLabel* actorHealthLabel;
	CTextLabel* actorScoreLabel;

	// Programs
	GLint program;
	GLint skyboxProgram;
	GLint enemyProgram;
	GLint pickupProgram;

	// Audio
	FMOD::System* audioSystem;

	// Utilities
	CSkybox* gameSkybox;
	CCamera* gameCamera;
	CInput* gameInput;
	Model* model;
	mat4 view;

	int gameSceneScore;
};

