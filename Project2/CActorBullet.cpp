#include "CActorBullet.h"

CActorBullet::CActorBullet(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture, CObject* gameActor)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{
	objPosition = gameActor->objPosition;
	actorVelocity = vec3(0.0f, 0.0f, 0.0f);
	previousActorVelocity = vec3(0.0f, 0.0f, 0.0f);
	objScaleAmount = 0.3f;
}

CActorBullet::~CActorBullet()
{
}

void CActorBullet::BulletUpdate(float mouseX, float mouseZ)
{
	vec3 actorDesiredVelocity;
	vec3 actorDesiredPoisiton = vec3(mouseX, 1.0f, mouseZ) - objPosition;

	actorDesiredVelocity = (glm::normalize(actorDesiredPoisiton) * maxSpeed);
	vec3 actorSteering;
	actorSteering = actorDesiredVelocity - actorVelocity;

	actorSteering = (glm::normalize(actorSteering) * clamp((float)glm::length(actorSteering), 0.0f, maxForce));
	actorVelocity += actorSteering;
	actorVelocity = (glm::normalize(actorVelocity) * clamp((float)glm::length(actorVelocity), 0.0f, maxSpeed));

	if (isnan(actorVelocity.x) or isnan(actorVelocity.y) or isnan(actorVelocity.z))
	{
		actorVelocity = previousActorVelocity;
	}
	else
	{
		previousActorVelocity = actorVelocity;
	}

	objPosition += actorVelocity;
}