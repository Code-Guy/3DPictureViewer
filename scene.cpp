#include "scene.h"
#include "camera.h"
#include "tool.h"

#include <algorithm>
#include <thread>
#include <mutex>

Camera *Scene::camera = NULL;
PointLight *Scene::light = NULL;

using namespace std;

const int ShadowMapSize = 2048;
const float SpanDistance = 3.5f;
const float MouseMoveSensitiy = 0.5f;
const float MouseEaseOutSensitiy = 3.0f;
const float MouseEaseOutDump = 0.01f;
const float BounceTimeInterval = 0.8f;
const int MaxVisiblePictureNum = 6;
const float CenterScale = 1.8f;
const float CenterAlpha = 0.4f;
const float ArrangePicturesInterval = 0.5f;

std::mutex mu;

Scene::Scene(int width, int height)
{
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
	loadFinish = false;
	arrangeFinish = false;

	loadBgPicture();

	std::thread t([this] { loadPictures(); });
	t.detach();
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
	static float arrangePicturesTimer = 0;
	if (!arrangeFinish)
	{
		arrangePicturesTimer += deltaTime;
		if (arrangePicturesTimer >= ArrangePicturesInterval)
		{
			arrangePictures();
			arrangePicturesTimer = 0;
		}
	}

	int n = pictures.size();
	if (n > 1)
	{
		float deltaAngle = 0;
		if (deltaMousePosX != 0)//如果拖动鼠标，旋转图片
		{
			lastDeltaMousePosX = deltaMousePosX;
			actions.clear();

			deltaAngle = -(float)deltaMousePosX / n * MouseMoveSensitiy;
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
		float upBoundAngle = spanAngle * (n / 2);
		float backAngle = spanAngle * n;
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
		if (!Tool::isFloatEqual(centerPictureAngle, 0) && abs(centerPictureAngle) < spanAngle / 2)
		{
			float imageAlpha = -2.0f * CenterAlpha * abs(centerPictureAngle) / spanAngle + CenterAlpha;
			float textAlpha = -2.0f * abs(centerPictureAngle) / spanAngle + 1;

			bgPicture->setAlpha(imageAlpha);
			emit setAlpha(textAlpha);
		}

		for (auto picture : pictures)
		{
			float curPictureAngle = picture->getAngle();
			if (Tool::isFloatEqual(curPictureAngle, 0))
			{
				picture->setSize(CenterScale);
			}
			else if (abs(curPictureAngle) < spanAngle / 2)
			{
				float size = 2.0f * (1 - CenterScale) / spanAngle * abs(curPictureAngle) + CenterScale;
				centerPicture->setSize(size);
			}
			else
			{
				picture->setSize(1.0f);
			}
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

void Scene::addAction()
{
	if (lastDeltaMousePosX != 0)
	{
		Action action;
		action.setBaseValue(0);
		float easeOutLen = lastDeltaMousePosX / pictures.size() * MouseEaseOutSensitiy;

		action.setIncrementValue(-easeOutLen);
		action.setTimeInterval(sqrt(abs(easeOutLen) * MouseEaseOutDump));
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

void Scene::arrangePictures()
{
	pictures.clear();
	mu.lock();
	int n = subthreadPictures.size();

	if (n != 0)
	{
		spanAngle = n <= 24 ? 15.0f : 360.0f / n;
		radius = 0.5f * SpanDistance / sin(glm::radians(spanAngle / 2));

		Picture::setRadius(radius);

		for (int i = 0; i < n; i++)
		{
			subthreadPictures[i]->glStuff();
			subthreadPictures[i]->setAngle(spanAngle * (i - n / 2));
			pictures.push_back(subthreadPictures[i]);
		}
	}
	mu.unlock();

	arrangeFinish = loadFinish;
}

void Scene::loadBgPicture()
{
	bgPicture = new Picture("Resources/bg/white.jpg");
	bgPicture->glStuff();
	bgPicture->setPosition(glm::vec3(0, 0, -1.0f));
	bgPicture->setSize(6);
	bgPicture->setVisible(true);
	bgPicture->setBlur(true);
	bgPicture->setAlpha(CenterAlpha);
}

void Scene::loadPictures()
{
	vector<string> picturePaths;
	//Tool::traverseFilesInDirectory("Resources/pictures", picturePaths, true);
	Tool::traverseFilesInDirectory("E:/Privacy/图片/基友西湖游", picturePaths, true);
	sort(picturePaths.begin(), picturePaths.end());

	int i = 0;
	for (string picturePath : picturePaths)
	{
		Picture *picture = new Picture(picturePath);
		if (i != 0 && subthreadPictures.size() == 0)//说明主线程已退出，工作线程没必要继续执行了
		{
			break;
		}

		mu.lock();
		subthreadPictures.push_back(picture);
		mu.unlock();

		i++;
	}

	loadFinish = true;
}