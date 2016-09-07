#include "scene.h"
#include "camera.h"
#include "tool.h"
#include <thread>
#include <algorithm>
#include <mutex>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>

Camera *Scene::camera = NULL;
PointLight *Scene::light = NULL;

using namespace std;

const int ShadowMapSize = 2048;//阴影贴图的大小（方形）
const float SpanDistance = 3.5f;//图片环中相邻图片的弦长距离
const float MousePressedMoveSensitivity = 0.5f;//鼠标按下移动的灵敏度
const float MouseReleasedMoveInSensitivity = 5.0f;//鼠标松开移动开始的灵敏度
const float MouseReleasedMoveOutSensitivity = 0.1f;//鼠标松开移动减速的灵敏度
const float BounceTimeInterval = 0.8f;//图片弹动的时间间隔
const int MaxVisiblePictureNum = 6;//最大可见图片数
const float CenterScale = 1.8f;//中心图片的放大系数
const float CenterAlpha = 0.4f;//中心图片的透明度
const float ArrangeTimeInterval = 0.5f;//初始化加载图片时排列图片的时间间隔
const float SwitchCenterPictureTimeInterval = 0.2f;//切换中心图片的时间间隔

std::mutex mu;

Scene::Scene(int width, int height) : width(width), height(height)
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

	billboardShader = new BillboardShader;
	//初始化billboard shader
	if (!billboardShader->init()) 
	{
		printf("Error initializing the billboardShader\n");
	}
	billboardShader->enable();
	billboardShader->setTextureUnit(0);

	overlayShader = new OverlayShader;
	if (!overlayShader->init())
	{
		printf("Error initializing the overlayShader\n");
	}
	overlayShader->enable();
	overlayShader->setScreenSize(glm::vec2(width, height));
	overlayShader->setTextureMap(0);

	shadowMapFBO = new ShadowMapFBO();
	if (!shadowMapFBO->init(width, height))
	{
		printf("initialize shadow map fbo failed!\n");
	}

	reset();
	loadBgPicture();
	initParticleSystems();
	initButtons(width, height);
}

Scene::~Scene()
{
	delete bgPicture;
	for (auto picture : pictures)
	{
		delete picture;
	}

	mu.lock();
	subthreadPictures.clear();
	mu.unlock();

	for (Button *button : buttons)
	{
		delete button;
	}

	for (int i = 0; i < 3; i++)
	{
		if (i != curPSIndex)
		{
			delete psButtons[i];
		}
	}

	for (Texture *psTexture : psTextures)
	{
		delete psTexture;
	}

	delete pictureShader;
	delete shadowMapShader;
	delete billboardShader;
	delete shadowMapFBO;
	delete overlayShader;
}

void Scene::logic(float deltaTime, int deltaMousePosX)
{
	//每隔一段时间重新排列一次图片，加载完成之后不再执行
	static float arrangeTimer = 0;
	if (!arrangeFinish)
	{
		arrangeTimer += deltaTime;
		if (arrangeTimer >= ArrangeTimeInterval)
		{
			arrangePictures();
			arrangeTimer = 0;
		}
	}

	//计算角度增量
	int n = pictures.size();
	if (n != 0)
	{
		float deltaAngle = 0;//角度增量
		if (deltaMousePosX != 0)//如果拖动鼠标，旋转图片
		{
			lastDeltaMousePosX = deltaMousePosX;
			actions.clear();

			deltaAngle = -(float)deltaMousePosX / n * MousePressedMoveSensitivity;
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
		float upBoundAngle = std::max(spanAngle * (n / 2), spanAngle);
		float backAngle = std::max(spanAngle * n, 2 * upBoundAngle);
		float invisibleAngle = MaxVisiblePictureNum / 2 * spanAngle;

		//更新每张图片的角度值
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

		//更新中心图片的文件信息
		centerPicture = pictures[curCenterPictureIndex];
		if (curCenterPictureIndex != prevCenterPictureIndex)
		{
			bgPicture->setBits(centerPicture->getBits(), centerPicture->getWidth(), centerPicture->getHeight());
			emit setFileName(centerPicture->getFileName());
			emit setResolution(centerPicture->getRealWidth(), centerPicture->getRealHeight());

			prevCenterPictureIndex = curCenterPictureIndex;
		}

		//设置图片透明度渐变效果
		if (!Tool::isFloatEqual(centerPictureAngle, 0) && abs(centerPictureAngle) < spanAngle / 2)
		{
			float imageAlpha = -2.0f * CenterAlpha * abs(centerPictureAngle) / spanAngle + CenterAlpha;
			float textAlpha = -2.0f * abs(centerPictureAngle) / spanAngle + 1;

			bgPicture->setAlpha(imageAlpha);
			emit setAlpha(textAlpha);
		}

		//设置图片大小渐变效果
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

		if (arrangeFinish)
		{
			bool isStatic = Tool::isFloatEqual(deltaAngle, 0);
			lps->setEmitting(isStatic);
			rps->setEmitting(isStatic);

			lps->logic(deltaTime);
			rps->logic(deltaTime);
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

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (arrangeFinish)
	{
		for (Button *button : buttons)
		{
			overlayShader->enable();
			button->render(overlayShader);
		}
		billboardShader->enable();
		lps->render(billboardShader);
		rps->render(billboardShader);
	}
	glDisable(GL_BLEND);
}

void Scene::mouseMove(int x, int y)
{
	if (arrangeFinish)
	{
		for (Button *button : buttons)
		{
			button->mouseMove(x, height - y);
		}
	}
}

void Scene::mousePress(int x, int y)
{
	if (arrangeFinish)
	{
		for (Button *button : buttons)
		{
			button->mousePress(x, height - y);
		}
	}
}

void Scene::mouseRelease(int x, int y)
{
	if (arrangeFinish)
	{
		for (Button *button : buttons)
		{
			button->mouseRelease(x, height - y);
		}
	}
}

void Scene::addEaseOutAction()
{
	if (!pictures.empty() && lastDeltaMousePosX != 0)
	{
		Action action;
		action.setBaseValue(0);
		float easeOutLen = lastDeltaMousePosX / pictures.size() * MouseReleasedMoveInSensitivity;

		action.setIncrementValue(-easeOutLen);
		action.setTimeInterval(sqrt(abs(easeOutLen)) * MouseReleasedMoveOutSensitivity);
		action.setCurveShape(EaseOutCurve);
		action.start();
		actions.push_back(action);

		lastDeltaMousePosX = 0;
	}
}

QString Scene::getCenterPicturePath(QPoint mousePos)
{
	if (pictures.empty() || !arrangeFinish)
	{
		return QString();
	}

	glm::vec3 p;
	glm::vec3 d;

	camera->castRay(glm::ivec4(0, 0, width, height), mousePos.x(), mousePos.y(), p, d);
	if (centerPicture->hit(p, d))
	{
		return centerPicture->getFilePath();
	}
	
	return QString();
}

void Scene::load(QString dir)
{
	if (!isLoading)
	{
		reset();

		bgPicture->setBlur(true);
		bgPicture->setAlpha(CenterAlpha);

		for (auto picture : pictures)
		{
			delete picture;
		}
		pictures.clear();
		subthreadPictures.clear();

		std::thread t(&Scene::loadPictures, this, dir);
		t.detach();
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

void Scene::rearrangePictures(int deletedPictureIndex)
{
	int n = pictures.size();
	if (n != 0)
	{
		spanAngle = n <= 24 ? 15.0f : 360.0f / n;
		radius = 0.5f * SpanDistance / sin(glm::radians(spanAngle / 2));

		Picture::setRadius(radius);

		for (int i = 0; i < n; i++)
		{
			pictures[i]->setAngle(spanAngle * (i - deletedPictureIndex));
		}
	}
}

void Scene::initParticleSystems()
{
	curPSIndex = 0;

	psTextures[0] = new Texture("Resources/particles/star.png");
	psTextures[1] = new Texture("Resources/particles/moon.png");
	psTextures[2] = new Texture("Resources/particles/sun.png");

	for (int i = 0; i < 3; i++)
	{
		psTextures[i]->attach();
	}
	
	glm::vec3 lpsColorSteps[4];
	lpsColorSteps[0] = glm::vec3(207, 24, 29) / 255.0;
	lpsColorSteps[1] = glm::vec3(144, 0, 123) / 255.0;
	lpsColorSteps[2] = glm::vec3(225, 143, 181) / 255.0;
	lpsColorSteps[3] = glm::vec3(230, 67, 112) / 255.0;

	lps = new ParticleSystem(glm::vec3(-1.8f, -1.5f, 0), 8, glm::vec3(0, -5.0f, 0),
		glm::vec3(0.1f, 0.1f, 0.1f), 0.8f,
		0.06f, 0.12f,
		0, 72,
		1.2f, 1.8f,
		glm::vec3(-0.8f, 2.0f, -0.2f), glm::vec3(-0.3f, 3.6f, 0.6f),
		lpsColorSteps);

	glm::vec3 rpsColorSteps[4];
	rpsColorSteps[0] = glm::vec3(27, 30, 83) / 255.0;
	rpsColorSteps[1] = glm::vec3(96, 173, 193) / 255.0;
	rpsColorSteps[2] = glm::vec3(181, 206, 236) / 255.0;
	rpsColorSteps[3] = glm::vec3(242, 214, 141) / 255.0;

	rps = new ParticleSystem(glm::vec3(1.8f, -1.5f, 0), 8, glm::vec3(0, -5.0f, 0),
		glm::vec3(0.1f, 0.1f, 0.1f), 0.8f,
		0.06f, 0.12f,
		0, 72,
		1.2f, 1.8f,
		glm::vec3(0.3f, 2.0f, -0.2f), glm::vec3(0.8f, 3.6f, 0.6f),
		rpsColorSteps);

	lps->setTexture(psTextures[curPSIndex]);
	rps->setTexture(psTextures[curPSIndex]);
}

void Scene::initButtons(int width, int height)
{
	Overlay::setScreenSize(glm::ivec2(width, height));

	buttons[0] = new Button("Resources/icons/help.png", glm::ivec2(20, 0));
	buttons[1] = new Button("Resources/icons/prev.png", glm::ivec2(405, 0));
	buttons[2] = new Button("Resources/icons/delete.png", glm::ivec2(502, 0)); 
	buttons[3] = new Button("Resources/icons/display.png", glm::ivec2(599, 0)); 
	buttons[4] = new Button("Resources/icons/edit.png", glm::ivec2(696, 0));
	buttons[5] = new Button("Resources/icons/next.png", glm::ivec2(793, 0));

	psButtons[0] = new Button("Resources/icons/star.png", glm::ivec2(1200, 0));
	psButtons[1] = new Button("Resources/icons/moon.png", glm::ivec2(1200, 0));
	psButtons[2] = new Button("Resources/icons/sun.png", glm::ivec2(1200, 0));

	buttons[6] = psButtons[curPSIndex];

	connect(buttons[0], SIGNAL(clicked()), this, SLOT(helpBtnClicked()));
	connect(buttons[1], SIGNAL(clicked()), this, SLOT(prevBtnClicked()));
	connect(buttons[2], SIGNAL(clicked()), this, SLOT(deleteBtnClicked()));
	connect(buttons[3], SIGNAL(clicked()), this, SLOT(displayBtnClicked()));
	connect(buttons[4], SIGNAL(clicked()), this, SLOT(editBtnClicked()));
	connect(buttons[5], SIGNAL(clicked()), this, SLOT(nextBtnClicked()));

	for (int i = 0; i < 3; i++)
	{
		connect(psButtons[i], SIGNAL(clicked()), this, SLOT(particleBtnClicked()));
	}
}

void Scene::reset()
{
	lastDeltaMousePosX = 0;
	prevCenterPictureIndex = -1;
	curCenterPictureIndex = -1;
	loadFinish = false;
	arrangeFinish = false;
	isLoading = false;
}

void Scene::loadBgPicture()
{
	bgPicture = new Picture("Resources/bg/splash.jpg");
	bgPicture->glStuff();
	bgPicture->setPosition(glm::vec3(0, 0, -1.0f));
	bgPicture->setSize(5);
	bgPicture->setVisible(true);
	bgPicture->setBlur(false);
	bgPicture->setAlpha(1.0f);
}

void Scene::loadPictures(QString dir)
{
	isLoading = true;
	vector<string> picturePaths;
	Tool::traverseFilesInDirectory(Tool::qstr2str(dir), picturePaths, true);
	sort(picturePaths.begin(), picturePaths.end());

	int i = 0;
	for (string picturePath : picturePaths)
	{
		Picture *picture = new Picture(picturePath);
		
		if (picture->isValid())
		{
			mu.lock();
			if (i != 0 && subthreadPictures.size() == 0)//说明主线程已退出，工作线程没必要继续执行了
			{
				mu.unlock();
				break;
			}
			subthreadPictures.push_back(picture);
			mu.unlock();
		}

		i++;
	}

	loadFinish = true;
	isLoading = false;
}

void Scene::reloadPicture(QString newCenterPicturePath)
{
	Picture *oldCenterPicture = pictures[curCenterPictureIndex];
	Picture *newCenterPicture = NULL;

	newCenterPicture = new Picture(Tool::qstr2str(newCenterPicturePath));
	newCenterPicture->glStuff();
	newCenterPicture->setAngle(0);

	bgPicture->setBits(newCenterPicture->getBits(), newCenterPicture->getWidth(), newCenterPicture->getHeight());
	emit setFileName(newCenterPicture->getFileName());
	emit setResolution(newCenterPicture->getRealWidth(), newCenterPicture->getRealHeight());

	pictures[curCenterPictureIndex] = newCenterPicture;
	delete oldCenterPicture;
}

void Scene::helpBtnClicked()
{
	emit showHelpDialog();
}

void Scene::prevBtnClicked()
{
	if (pictures.size() > 1)
	{
		if (!actions.empty() && actions.back().getCurveShape() == LinearCurve)
		{
			return;
		}
		actions.clear();

		Action action;
		action.setBaseValue(0);
		Picture *prevPicture = pictures[(curCenterPictureIndex + 1) % pictures.size()];
		action.setIncrementValue(-prevPicture->getAngle());
		action.setTimeInterval(SwitchCenterPictureTimeInterval);
		action.setCurveShape(LinearCurve);
		action.start();
		actions.push_back(action);
	}
}

void Scene::deleteBtnClicked()
{
	if (!pictures.empty())
	{
		Tool::moveToTrash(centerPicture->getFilePath());
		pictures.erase(pictures.begin() + curCenterPictureIndex);
		curCenterPictureIndex = curCenterPictureIndex == 0 ? pictures.size() : curCenterPictureIndex - 1;
		rearrangePictures(curCenterPictureIndex);
	}
}

void Scene::displayBtnClicked()
{
	if (!pictures.empty())
	{
		emit displayCenterPicture(centerPicture->getFilePath());
	}
}

void Scene::editBtnClicked()
{
	if (!pictures.empty())
	{
		emit showEditPictureDialog(centerPicture->getPath(), centerPicture->getFileBaseName(), centerPicture->getFileSuffix(), 
			centerPicture->getRealWidth(), centerPicture->getRealHeight());
	}
}

void Scene::nextBtnClicked()
{
	if (pictures.size() > 1)
	{
		if (!actions.empty() && actions.back().getCurveShape() == LinearCurve)
		{
			return;
		}
		actions.clear();

		Action action;
		action.setBaseValue(0);
		Picture *prevPicture = pictures[(curCenterPictureIndex - 1 + pictures.size()) % pictures.size()];
		action.setIncrementValue(-prevPicture->getAngle());
		action.setTimeInterval(SwitchCenterPictureTimeInterval);
		action.setCurveShape(LinearCurve);
		action.start();
		actions.push_back(action);
	}
}

void Scene::particleBtnClicked()
{
	curPSIndex = (curPSIndex + 1) % 3;
	lps->setTexture(psTextures[curPSIndex]);
	rps->setTexture(psTextures[curPSIndex]);
	buttons[6] = psButtons[curPSIndex];
}