#include "scene.h"
#include "camera.h"
#include "tool.h"

#include <algorithm>

Camera *Scene::camera = NULL;
PointLight *Scene::light = NULL;

using namespace std;

const int ShadowMapSize = 2048;
const float SpanDistance = 3.5f;
const float MouseMoveSensitiy = 0.02f;
const float MouseEaseOutBound = 5.0f;
const float MouseEaseOutSensitiy = 0.5f;
const float MouseEaseOutSpeed = 100.0f;
const float BounceTimeInterval = 0.8f;
const int MaxVisiblePictureNum = 6;
const float ScaleAngle = 10.0f;
const float CenterScale = 1.8f;
const float CenterAlpha = 0.4f;

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

	prevCenterPictureIndex = -1;
	curCenterPictureIndex = -1;
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
	if (deltaMousePosX != 0)//如果拖动鼠标，旋转图片
	{
		lastDeltaMousePosX = deltaMousePosX;
		actions.clear();

		deltaAngle = -deltaMousePosX * MouseMoveSensitiy;
	}
	else if (!actions.empty())//执行动作队列
	{
		Action &action = actions.front();
		if (!action.isRunning())
		{
			if (action.getCurveShape() == EaseOutCurve)
			{
				Action bounceAction;
				bounceAction.setBaseValue(0);
				bounceAction.setIncrementValue(-centerPictureAngle);
				bounceAction.setTimeInterval(BounceTimeInterval);
				bounceAction.setCurveShape(BounceCurve);
				bounceAction.start();
				actions.push_back(bounceAction);
			}
			actions.pop_front();
		}
		else
		{
			action.logic(deltaTime);
			deltaAngle = action.getDeltaValue();
		}
	}

	//计算各种特殊角度
	float upBoundAngle = spanAngle * (pictures.size() / 2);
	float backAngle = spanAngle * pictures.size();
	float invisibleAngle = MaxVisiblePictureNum / 2 * spanAngle;

	centerPictureAngle = MAX_NUM;
	int index = 0;
	for (auto picture : pictures)
	{
		picture->addAngle(deltaAngle);
		//循环显示
		if (picture->getAngle() > upBoundAngle)
		{
			picture->addAngle(-backAngle);
		}
		else if (picture->getAngle() < -(backAngle - upBoundAngle))
		{
			picture->addAngle(backAngle);
		}
		//不显示摄像机不在视野内的图片
		if (abs(picture->getAngle()) < invisibleAngle)
		{
			picture->setVisible(true);
		}

		if (abs(picture->getAngle()) < abs(centerPictureAngle))
		{
			centerPictureAngle = picture->getAngle();
			curCenterPictureIndex = index;
		}
		index++;
	}

	Picture *centerPicture = pictures[curCenterPictureIndex];
	//如果中心图片发生变动
	if (curCenterPictureIndex != prevCenterPictureIndex)
	{
		bgPicture->setBits(centerPicture->getBits(), centerPicture->getWidth(), centerPicture->getHeight());
		emit setFileName(centerPicture->getFileName());
		emit setResolution(centerPicture->getRealWidth(), centerPicture->getRealHeight());

		prevCenterPictureIndex = curCenterPictureIndex;
	}
	//各种渐变效果
	if (!Tool::isFloatEqual(centerPictureAngle, 0) && abs(centerPictureAngle) < ScaleAngle)
	{
		float size = (1 - CenterScale) / ScaleAngle * abs(centerPictureAngle) + CenterScale;
		float imageAlpha = -CenterAlpha * abs(centerPictureAngle) / ScaleAngle + CenterAlpha;
		float textAlpha = -abs(centerPictureAngle) / ScaleAngle + 1;

		centerPicture->setSize(size);
		bgPicture->setAlpha(imageAlpha);
		emit setAlpha(textAlpha);
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

void Scene::addAction()
{
	if (lastDeltaMousePosX != 0)
	{
		Action action;
		action.setBaseValue(0);
		float mouseEaseOutAngle = lastDeltaMousePosX * MouseEaseOutSensitiy;
		action.setIncrementValue(-mouseEaseOutAngle);
		action.setTimeInterval(abs(mouseEaseOutAngle / MouseEaseOutSpeed));
		action.setCurveShape(EaseOutCurve);
		action.start();
		actions.push_back(action);

		lastDeltaMousePosX = 0;
	}
}

void Scene::initSingletons(int width, int height)
{
	camera = new Camera(glm::vec3(0, 0, 4), glm::vec3(), glm::vec3(0, 1, 0), (float)width / height);

	light = new PointLight;
	light->position = glm::vec3(0.0f, 1.5f, 5.5f);
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
	bgPicture->setPosition(glm::vec3(0, 0, -1.0f));
	bgPicture->setSize(6);
	bgPicture->setVisible(true);
	
	vector<string> picturePaths;
	Tool::traverseFilesInDirectory("Resources/pictures", picturePaths, true);
	//Tool::traverseFilesInDirectory("E:/Privacy/图片/基友西湖游", picturePaths, true);

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

	Picture *centerPicture = pictures[n / 2];
	centerPicture->setSize(CenterScale);
	bgPicture->setBlur(true);
	bgPicture->setAlpha(CenterAlpha);
}