#ifndef _SCENE_H
#define _SCENE_H

#include "picture.h"
#include "pictureshader.h"
#include "shadowmapshader.h"
#include "shadowmapfbo.h"
#include "camera.h"
#include "light.h"
#include "action.h"

#include <vector>
#include <deque>
#include <QObject>

class Scene : public QObject
{
	Q_OBJECT
public:
	Scene(int width, int height);
	~Scene();

	void logic(float deltaTime, int deltaMousePosX);
	void render();

	void addAction();
	QString getCenterPicturePath();

	static void initSingletons(int width, int height);
	static void destorySingletons();

	static Camera *getCamera();
	static PointLight *getLight();

signals:
	void setFileName(QString fileName);
	void setResolution(int width, int height);
	void setAlpha(float alpha);

private:
	Picture *bgPicture;
	std::vector<Picture *> subthreadPictures;
	std::vector<Picture *> pictures;

	Picture *centerPicture;

	static Camera *camera;
	static PointLight *light;

	PictureShader *pictureShader;
	ShadowMapShader *shadowMapShader;
	ShadowMapFBO *shadowMapFBO;

	float radius;
	float spanAngle;

	float lastDeltaMousePosX;

	int prevCenterPictureIndex;
	int curCenterPictureIndex;
	float centerPictureAngle;

	std::deque<Action> actions;

	bool loadFinish;
	bool arrangeFinish;

	void loadBgPicture();
	void loadPictures();
	void arrangePictures();
};

#endif //_SCENE_H