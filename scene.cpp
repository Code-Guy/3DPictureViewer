#include "scene.h"
#include "camera.h"
#include "tool.h"

#include <algorithm>

Camera *Scene::camera = NULL;
PointLight *Scene::light = NULL;

using namespace std;

const int ShadowMapSize = 2048;
const float SpanDistance = 2.5f;
const float MouseMoveSensitiy = 0.05f;
const float MouseEaseOutSensitiy = 0.2f;
const float MouseEaseOutSpeed = 5.0f;

Scene::Scene(int width, int height)
{
	genPictures();

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

	shadowMapFBO = new ShadowMapFBO();
	if (!shadowMapFBO->init(width, height))
	{
		printf("initialize shadow map fbo failed!\n");
	}
}

Scene::~Scene()
{
	delete bgPicture;
	for (auto picture : pictures)
	{
		delete picture;
	}

	delete pictureShader;
	delete shadowMapShader;
	delete shadowMapFBO;
}

void Scene::logic(float deltaTime, int deltaMousePosX)
{
	float deltaAngle = 0;
	if (deltaMousePosX != 0)
	{
		lastDeltaMousePosX = deltaMousePosX;
		actions.clear();

		deltaAngle = deltaMousePosX * MouseMoveSensitiy;
	}

	if (!actions.empty())
	{
		Action &action = actions.front();
		if (!action.isRunning())
		{
			actions.pop_front();
		}
		else
		{
			action.logic(deltaTime);
			deltaAngle = action.getDeltaValue();
		}
	}

	float upBoundAngle = spanAngle * (pictures.size() / 2);
	float backAngle = spanAngle * pictures.size();

	for (auto picture : pictures)
	{
		picture->addAngle(-deltaAngle);
		if (picture->getAngle() > upBoundAngle)
		{
			picture->addAngle(-backAngle);
		}
		else if (picture->getAngle() < -(backAngle - upBoundAngle))
		{
			picture->addAngle(backAngle);
		}
	}
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pictureShader->enable();
	shadowMapFBO->bindForReading(GL_TEXTURE1);

	bgPicture->renderPass(pictureShader);
	for (auto picture : pictures)
	{
		picture->renderPass(pictureShader);
	}
}

void Scene::addEaseOutAction()
{
	Action action;
	action.setBaseValue(0);
	float mouseEaseOutAngle = lastDeltaMousePosX * MouseEaseOutSensitiy;
	action.setIncrementValue(mouseEaseOutAngle);
	action.setTimeInterval(mouseEaseOutAngle / MouseEaseOutSpeed);
	action.setCurveShape(EaseOutCurve);
	action.start();
	actions.push_back(action);
}

void Scene::initSingletons(int width, int height)
{
	camera = new Camera(glm::vec3(0, 0, 4), glm::vec3(), glm::vec3(0, 1, 0), (float)width / height);

	light = new PointLight;
	light->position = glm::vec3(0.0f, 0.5f, 5.5f);
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

PointLight *Scene::getLight()
{
	return light;
}

void Scene::genPictures()
{
	bgPicture = new Picture("Resources/bg/white.jpg");
	bgPicture->setPosition(glm::vec3(0, 0, -2.0f));
	bgPicture->setSize(7);

	vector<string> picturePaths;
	Tool::traverseFilesInDirectory("Resources/pictures", picturePaths, true);

	sort(picturePaths.begin(), picturePaths.end());

	spanAngle = picturePaths.size() <= 24 ? 15.0f : 360.0f / picturePaths.size();
	radius = 0.5f * SpanDistance / sin(glm::radians(spanAngle / 2));

	Picture::setRadius(radius);

	int n = picturePaths.size();
	for (int i = 0; i < n; i++)
	{
		Picture *picture = new Picture(picturePaths[i]);
		picture->addAngle(spanAngle * (i - n / 2));

		pictures.push_back(picture);
	}
}