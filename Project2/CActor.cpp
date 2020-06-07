#include "CActor.h"

CActor::CActor(GLint* _program, GLuint* _VAO, int _indiceCount, CCamera* _gameCamera, GLuint* _texture)
	: CObject(_program, _VAO, _indiceCount, _gameCamera, _texture)
{
	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/Blinn-Phong.fs");
	
	// Gen Textures For Actor
	const char* fileLocationBullet = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocationBullet, &texture);
	
	actorSphere = new CSphere();
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

void CActor::ShootInput(GLfloat deltaTime, CInput* gameInput)
{
	float mouseX = gameInput->getMouseX();
	float mouseY = gameInput->getMouseY();
	
	if (gameInput->getClick(0))
	{
		vec2 mousePos
		{
			mouseX, mouseY,
		};
	
		newBullet = new CActorBullet(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &texture);
		bulletsInScene.insert(std::make_pair(newBullet, mousePos));
	
		std::cout << "Bullet Created" << std::endl;
	}
}

void CActor::BulletUpdate()
{
	for (std::map<CActorBullet*, vec2>::iterator bulletIndex = bulletsInScene.begin(); bulletIndex != bulletsInScene.end(); ++bulletIndex)
	{
		bulletIndex->first->FirstShot(bulletIndex->second.x, bulletIndex->second.y);
		
		bulletIndex->first->Update();
	}
}

void CActor::BulletRender()
{
	for (std::map<CActorBullet*, vec2>::iterator bulletIndex = bulletsInScene.begin(); bulletIndex != bulletsInScene.end(); ++bulletIndex)
	{
		bulletIndex->first->FirstShot(bulletIndex->second.x, bulletIndex->second.y);

		bulletIndex->first->Render();
	}
}

void CActor::TextureGen(const char* textureLocation, GLuint* texture)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	
	int width, height;
	unsigned char* image1 = SOIL_load_image(textureLocation, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}