#pragma once
#include "CObject.h"
#include "CShaderLoader.h"
#include "CPyramid.h"

class CActorPickupScore : public CObject
{
public:
	CActorPickupScore(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture);
	~CActorPickupScore();

	void TextureGen(const char* textureLocation, GLuint* texture);

private:

	CPyramid* actorPyramid;
	GLuint texture;
	GLint program;
};