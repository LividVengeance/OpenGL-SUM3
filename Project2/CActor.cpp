#include "CActor.h"

CActor::CActor(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{

}

void CActor::MoveInput(GLfloat deltaTime, CInput* gameInput)
{
	// Move Right
	if (gameInput->getKeyState('D') == true || gameInput->getKeyState('d') == true)
	{
		objPosition.x += 20.0f * deltaTime;
	}
	// Move Left
	if (gameInput->getKeyState('A') == true || gameInput->getKeyState('a') == true)
	{
		objPosition.x -= 20.0f * deltaTime;
	}
	// Move Up
	if (gameInput->getKeyState('W') == true || gameInput->getKeyState('w') == true)
	{
		objPosition.z -= 20.0f * deltaTime;
	}
	// Move Up
	if (gameInput->getKeyState('S') == true || gameInput->getKeyState('s') == true)
	{
		objPosition.z += 20.0f * deltaTime;
	}
}