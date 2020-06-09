#pragma once
#include "CObject.h"

class CActorPickupHealth : public CObject
{
public:
	CActorPickupHealth(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture);
	~CActorPickupHealth();

private:

};

