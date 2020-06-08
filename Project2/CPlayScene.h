#pragma once

#include "CShaderLoader.h"
#include "CCamera.h"
#include "CInput.h"
#include "CTextLabel.h"

#include "CActorEnemy.h"
#include "CActor.h"
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

	void TextureGen(const char* textureLocation, GLuint* texture);
	void CollisionCheck(CActor* actorOne, CObject* actorTwo);
	void ResetScene();
	int GetPlayerScore();


	ESceneManager* currentScene;

	// Actor
	GLuint actorTex;
	CPyramid* actorEnemyPyramid;
	CSphere* actorSphere;
	CActor* gameActor;
	CActorEnemy* actorEnemy;
	CSkybox* gameSkybox;

	// Labels
	CTextLabel* actorHealthLabel;
	CTextLabel* actorScoreLabel;

	GLint program;
	GLint skyboxProgram;
	GLint enemyProgram;

	FMOD::System* audioSystem;

	CCamera* gameCamera;
	CInput* gameInput;
	Model* model;
	mat4 view;
};

