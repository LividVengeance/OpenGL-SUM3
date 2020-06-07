#include "CInput.h"

CInput::CInput()
{

}

CInput::~CInput()
{
}

void CInput::ProcessInput()
{
}

void CInput::MouseClick(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	if (button >= 3)
	{
		return;
	}

	MouseState[button] = ((state == GLUT_DOWN) ? INPUT_DOWN : INPUT_UP);
}

void CInput::KeyboardDown(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_DOWN;
}

void CInput::KeyboardUp(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_UP;
}

void CInput::MousePassiveMove(int x, int y)
{
	//std::cout << "Passive x: " << x << " | y: " << y << std::endl;
}

void CInput::MouseMove(int x, int y)
{
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
}

bool CInput::getKeyState(char key)
{
	// Gets KeyBoard State
	if (KeyState[key] == INPUT_DOWN)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

int CInput::getMouseX()
{
	//float offSetX = Utils::SCR_WIDTH / 2;
	//mouseX = offSetX - mouseX;
	return(mouseX);
}

int CInput::getMouseY()
{
	//float offSetY = Utils::SCR_HEIGHT / 2;
	//mouseY = offSetY - mouseY;
	return(mouseY);
}

bool CInput::getClick(int button)
{
	if (MouseState[button] == INPUT_DOWN)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}