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

class Scene
{
public:
	Scene(int width, int height);
	~Scene();

	void logic(float deltaTime, int deltaMousePosX);
	void render();

	void addEaseOutAction();

	static void initSingletons(int width, int height);
	static void destorySingletons();

	static Camera *getCamera();
	static PointLight *getLight();

private:
	Picture *bgPicture;
	std::vector<Picture *> pictures;

	static Camera *camera;
	static PointLight *light;

	PictureShader *pictureShader;
	ShadowMapShader *shadowMapShader;
	ShadowMapFBO *shadowMapFBO;

	float radius;
	float spanAngle;

	float lastDeltaMousePosX;

	std::deque<Action> actions;

	void genPictures();
};

#endif //_SCENE_H