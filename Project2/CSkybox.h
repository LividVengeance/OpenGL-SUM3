#pragma once
#include "CCamera.h"

class CSkybox
{
public:
	CSkybox(GLint* _program, CCamera* gameCamera);
	~CSkybox();

	void Render();
	void Update();

	CCamera* gameCamera;
	GLuint texID;
	GLint* program;
	mat4 MVP;
	GLuint VAO, EBO, VBO;

private:

};

