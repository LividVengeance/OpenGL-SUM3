#pragma once
#include "CObject.h"
#include "CShaderLoader.h"
#include "CPyramid.h"

class CActorPickup : public CObject
{
public:
	CActorPickup(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture);
	~CActorPickup();

	void TextureGen(const char* textureLocation, GLuint* texture);

private:

	CPyramid* actorPyramid;
	GLuint texture;
	GLint program;
};