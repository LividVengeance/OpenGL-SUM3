#include "CActorBullet.h"

CActorBullet::CActorBullet(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{
	objPosition = vec3(0.0f, 0.0f, 0.0f);
	actorVelocity = vec3(10.0f, 10.0f, 10.0f);
	previousActorVelocity = vec3(0.0f, 0.0f, 0.0f);
}

CActorBullet::~CActorBullet()
{
}

void CActorBullet::Update()
{

}

void CActorBullet::FirstShot(float mouseX, float mouseY)
{
	vec3 actorDesiredVelocity;
	vec3 actorDesiredPoisiton = vec3(mouseX, mouseY, 1.0f) - objPosition;

	actorDesiredVelocity = glm::normalize(actorDesiredPoisiton) * maxSpeed;
	vec3 actorSteering = actorDesiredVelocity - actorVelocity;

	actorSteering = glm::normalize(actorSteering) * clamp((float)glm::length(actorSteering), 0.0f, maxForce);
	actorVelocity += actorSteering;
	actorVelocity = glm::normalize(actorVelocity) * clamp((float)glm::length(actorVelocity), 0.0f, maxSpeed);

	if (isnan(actorVelocity.x) or isnan(actorVelocity.y))
	{
		actorVelocity = previousActorVelocity;
	}
	else
	{
		previousActorVelocity = actorVelocity;
	}

	objPosition += actorVelocity;
}