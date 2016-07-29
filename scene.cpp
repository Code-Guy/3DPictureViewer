#include "scene.h"
#include "camera.h"

Scene::Scene(int width, int height)
{
	Picture *bgPicture = new Picture("Resources/pictures/white.jpg");
	bgPicture->setPosition(glm::vec3(0, 0, -1.0f));
	bgPicture->setSize(100);

	Picture *displayPicture = new Picture("Resources/pictures/aurora.jpg");

	pictures.push_back(bgPicture);
	pictures.push_back(displayPicture);

	pictureShader = new PictureShader;
	if (!pictureShader->init())
	{
		printf("initialize picture shader failed!\n");
	}
	pictureShader->setTextureMap(0);

	Camera::setCamera(glm::vec3(0, 0, 5.0f), glm::vec3(), glm::vec3(0, 1.0f, 0), (float)width / height);
}

Scene::~Scene()
{
	Camera::destoryCamera();

	for (auto picture : pictures)
	{
		delete picture;
	}

	delete pictureShader;
}

void Scene::logic(float deltaTime)
{
	Camera::getCamera()->logic(deltaTime);

	pictures[1]->addRotation(glm::vec3(0, 0, deltaTime * 2.0f));
}

void Scene::render()
{
	for (auto picture : pictures)
	{
		picture->render(pictureShader);
	}
}