#include "CActorEnemy.h"

CActorEnemy::CActorEnemy(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{
	objPosition = vec3(0.0f, 0.0f, 0.0f);
	actorVelocity = vec3(0.0f, 0.0f, 0.0f);
	previousActorVelocity = vec3(0.0f, 0.0f, 0.0f);
}

void CActorEnemy::SteeringSeek(GLfloat deltaTime, CObject* gameActor)
{
	vec3 actorDesiredVelocity;
	vec3 actorDesiredPoisiton = gameActor->objPosition - objPosition;

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