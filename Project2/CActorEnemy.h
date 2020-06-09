#pragma once
#include "CObject.h"

class CActorEnemy : public CObject
{
public:
	CActorEnemy(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture);

	void SteeringSeek(GLfloat deltaTime, CObject* gameActor);
	void MoveInput(GLfloat deltaTime, CInput* gameInput);

	vec3 previousActorVelocity;
	vec3 actorVelocity;
	float maxSpeed = 0.05f;
	float maxForce = 0.2f;
};