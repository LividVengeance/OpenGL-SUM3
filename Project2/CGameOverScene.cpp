#include "CGameOverScene.h"

CGameOverScene::CGameOverScene(CCamera* _gameCamera, CInput* _gameInput)
{
	gameCamera = _gameCamera;
	gameInput = _gameInput;

	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic.vs",
		"Resources/Shaders/Basic.fs");

	// Gen Textures For Buttons
	const char* fileLocationPlay = "Resources/Textures/MainMenu/playSprite.png";
	TextureGen(fileLocationPlay, &playButtonTex);
	const char* fileLocationExit = "Resources/Textures/MainMenu/exitSprite.png";
	TextureGen(fileLocationExit, &menuButtonTex);
	const char* fileLocationTitle = "Resources/Textures/MainMenu/titleSprite.png";
	TextureGen(fileLocationTitle, &titleTex);
	// Create Type
	buttonPlane = new CPlane(150.0f, 50.0f);
	titlePlane = new CPlane(200.0f, 50.0f);

	playButtonObj = new CObject(&program, buttonPlane->GetVAO(), buttonPlane->GetIndiceCount(), gameCamera, &playButtonTex);
	menuButtonObj = new CObject(&program, buttonPlane->GetVAO(), buttonPlane->GetIndiceCount(), gameCamera, &menuButtonTex);
	titleObj = new CObject(&program, titlePlane->GetVAO(), titlePlane->GetIndiceCount(), gameCamera, &titleTex);

	playButtonObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 200;
	playButtonObj->objPosition.y += (Utils::SCR_HEIGHT / 2) - 150;

	menuButtonObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 200;
	menuButtonObj->objPosition.y += (Utils::SCR_HEIGHT / 2) - 200;

	titleObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 225;
	titleObj->objPosition.y += (Utils::SCR_HEIGHT / 2) - 75;
}

CGameOverScene::~CGameOverScene()
{
}

void CGameOverScene::Update(GLfloat* deltaTime, ESceneManager* _currentScene)
{

}

void CGameOverScene::Render()
{

}

bool CGameOverScene::Button(float width, float height, CObject* _buttonObj)
{
	mouseX = gameInput->getMouseX();
	mouseY = gameInput->getMouseY();

	float offSetX = Utils::SCR_WIDTH / 2;
	float offSetY = Utils::SCR_HEIGHT / 2;

	if (-(offSetX - mouseX) > _buttonObj->objPosition.x - width / 2 &&
		-(offSetX - mouseX) < _buttonObj->objPosition.x + width / 2 &&
		offSetY - mouseY > _buttonObj->objPosition.y - height / 2 &&
		offSetY - mouseY < _buttonObj->objPosition.y + height / 2 &&
		(gameInput->getClick(0) || gameInput->getClick(1)))
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void CGameOverScene::TextureGen(const char* textureLocation, GLuint* texture)
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