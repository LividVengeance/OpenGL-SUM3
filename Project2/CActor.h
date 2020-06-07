#pragma once
#include "CObject.h"

class CActor : public CObject
{
public:
	CActor(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture);
	void MoveInput(GLfloat deltaTime, CInput* gameInput);

	int actorHealth = 10;
	int actorScore = 0;
};