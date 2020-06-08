#pragma once
#include "CObject.h"
#include "CActorBullet.h"
#include "CSphere.h"
#include "CShaderLoader.h"
#include "CAudio.h"

#include <map>

class CActor : public CObject
{
public:
	CActor(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture, FMOD::System* _audioSystem);
	~CActor();

	void MoveInput(GLfloat deltaTime, CInput* gameInput);
	void ShootInput(GLfloat deltaTime, CInput* gameInput);

	void TextureGen(const char* textureLocation, GLuint* texture);

	void BulletUpdate();
	void BulletRender();

	std::map<CActorBullet*, vec2> bulletsInScene;
	int shootDist = 50;
	
	CAudio* shootTrack;

	CActorBullet* newBullet;

	CSphere* actorSphere;
	GLuint texture;
	GLint program;

	FMOD::System* audioSystem;

	int actorHealth = 10;
	int actorScore = 0;
};