#include "CPlayScene.h"

CPlayScene::CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem)
{
	gameCamera = _gameCamera;
	gameInput = _gameInput;
	audioSystem = _audioSystem;

	skyboxProgram = CShaderLoader::CreateProgram("Resources/Shaders/skybox.vs",
		"Resources/Shaders/skybox.fs");

	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/Blinn-Phong.fs");

	enemyProgram = CShaderLoader::CreateProgram("Resources/Shaders/Basic-Normal.vs",
		"Resources/Shaders/RimLighting.fs");

	// Gen Textures For Actor
	const char* fileLocationPlay = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocationPlay, &actorTex);

	gameSkybox = new CSkybox(&skyboxProgram, gameCamera);
	actorSphere = new CSphere();
	
	model = new Model("Resources/Models/Tank/Tank.obj", gameCamera);
	actorEnemyPyramid = new CPyramid();
	actorEnemy = new CActorEnemy(&enemyProgram, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex);

	
	gameActor = new CActor(&program, actorSphere->GetVAO(), actorSphere->GetIndiceCount(), gameCamera, &actorTex, audioSystem);

	// Labels
	actorHealthLabel = new CTextLabel("Health: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 560.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	actorScoreLabel = new CTextLabel("Score: ", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 520.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Render()
{
	glUseProgram(program);

	gameSkybox->Render();

	// Actors
	gameActor->Render();
	actorEnemy->Render();

	gameActor->BulletRender(); // Renders all the bullets in the scene

	//model->Render(actorEnemy);
	// Labels
	actorHealthLabel->Render();
	actorScoreLabel->Render();

	glBindVertexArray(0);		// Unbinding VAO
	glUseProgram(0);
}

void CPlayScene::Update(GLfloat* deltaTime, ESceneManager* _currentScene)
{
	currentScene = _currentScene;
	gameCamera->Update(*deltaTime);
	gameSkybox->Update();

	// Actors
	gameActor->Update();
	gameActor->MoveInput(*deltaTime, gameInput);

	gameActor->ShootInput(*deltaTime, gameInput); // Creates bullet on mouse click
	gameActor->BulletUpdate(); // Updates all the bullents in scene

	actorEnemy->Update();
	actorEnemy->SteeringSeek(*deltaTime, gameActor);

	// Updates the score label
	std::string scoreStr = "Score: ";
	scoreStr += std::to_string(gameActor->actorScore);
	actorScoreLabel->SetText(scoreStr);

	// Updates the health label
	std::string healthStr = "Health: ";
	healthStr += std::to_string(gameActor->actorHealth);
	actorHealthLabel->SetText(healthStr);
}

void CPlayScene::TextureGen(const char* textureLocation, GLuint* texture)
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