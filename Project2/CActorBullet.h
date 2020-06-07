#pragma once
#include "CObject.h"
#include "CSphere.h"

class CActorBullet : public CObject
{
public:
	CActorBullet(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture, CObject* gameActor);
	~CActorBullet();

	void BulletUpdate(float mouseX, float mouseZ);

	vec3 previousActorVelocity;
	vec3 actorVelocity;
	float maxSpeed = 0.1f;;
	float maxForce = 0.2f;;
protected:
};

