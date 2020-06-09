#include "CActorPickupHealth.h"

CActorPickupHealth::CActorPickupHealth(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{
}

CActorPickupHealth::~CActorPickupHealth()
{
}