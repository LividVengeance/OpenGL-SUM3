#pragma once
#include "CCamera.h"

class CPyramid
{
public:
	CPyramid();
	~CPyramid();

	GLuint* GetVAO();
	int GetIndiceCount();

	GLuint VAO;
	float indiceCount;

private:

};

