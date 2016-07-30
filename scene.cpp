#include "scene.h"
#include "camera.h"

Camera *Scene::camera = NULL;
DirectionLight *Scene::light = NULL;

Scene::Scene(int width, int height)
{
	Picture *bgPicture = new Picture("Resources/pictures/white.jpg");
	bgPicture->setPosition(glm::vec3(0, 0, -0.5f));
	bgPicture->setSize(100);

	Picture *displayPicture = new Picture("Resources/pictures/aurora.jpg");

	pictures.push_back(bgPicture);
	pictures.push_back(displayPicture);

	pictureShader = new PictureShader;
	if (!pictureShader->init())
	{
		printf("initialize picture shader failed!\n");
	}
	pictureShader->enable();
	pictureShader->setTextureMap(0);
	pictureShader->setShadowMap(1);

	shadowMapShader = new ShadowMapShader;
	if (!shadowMapShader->init())
	{
		printf("initialize shadow map shader failed!\n");
	}

	shadowMapFBO = new ShadowMapFBO;
	if (!shadowMapFBO->init(width, height))
	{
		printf("initialize shadow map fbo failed!\n");
	}
}

Scene::~Scene()
{
	for (auto picture : pictures)
	{
		delete picture;
	}

	delete pictureShader;
	delete shadowMapShader;
	delete shadowMapFBO;
}

void Scene::logic(float deltaTime)
{
	camera->logic(deltaTime);

	//pictures[1]->addRotation(glm::vec3(0, 0, deltaTime * 2.0f));
}

void Scene::render()
{
	shadowMapFBO->bindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapShader->enable();

	for (auto picture : pictures)
	{
		picture->shadowMapPass(shadowMapShader);
	}

	shadowMapFBO->unBind();

	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pictureShader->enable();
	shadowMapFBO->bindForReading(GL_TEXTURE1);

	for (auto picture : pictures)
	{
		picture->renderPass(pictureShader);
	}
}

void Scene::initSingletons(int width, int height)
{
	camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(), glm::vec3(0, 1, 0), (float)width / height);

	light = new DirectionLight;
	light->position = glm::vec3(1.0f, 1.0f, 2.0f);
	light->target = glm::vec3();
}

void Scene::destorySingletons()
{
	delete camera;
	delete light;
}

Camera *Scene::getCamera()
{
	return camera;
}

DirectionLight *Scene::getLight()
{
	return light;
}