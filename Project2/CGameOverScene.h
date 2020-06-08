#pragma once
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <fmod.hpp>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Utilities.h"
#include "CObject.h"
#include "CPlane.h"
#include "CShaderLoader.h"
#include "CTextLabel.h"

class CGameOverScene
{
public:
	CGameOverScene(CCamera* _gameCamera, CInput* _gameInput, int actorHealth);
	~CGameOverScene();

	void Render();
	void Update(GLfloat* deltaTime, ESceneManager* _currentScene);

	void TextureGen(const char* textureLocation, GLuint* texture);
	bool Button(float width, float height, CObject* _buttonObj);

	int mouseX;
	int mouseY;

	float offSetX = Utils::SCR_WIDTH / 2;
	float offSextY = Utils::SCR_HEIGHT / 2;

	CCamera* gameCamera;

	GLint program;

	ESceneManager* currentScene;

	// Button Textures
	GLuint playButtonTex;
	GLuint menuButtonTex;
	GLuint scoreTex;
	GLuint titleTex;
	// Button Objects
	CObject* playButtonObj;
	CObject* menuButtonObj;
	CObject* scoreObj;
	CObject* titleObj;
	// Button Type
	CPlane* buttonPlane;
	CPlane* titlePlane;
	// Label
	CTextLabel* scoreLabel;

	CInput* gameInput;

private:
	std::string playerScore;
};

