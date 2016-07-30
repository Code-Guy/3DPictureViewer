#ifndef _SCENE_H
#define _SCENE_H

#include "picture.h"
#include "pictureshader.h"
#include "shadowmapshader.h"
#include "shadowmapfbo.h"
#include "camera.h"
#include "light.h"

#include <vector>

class Scene
{
public:
	Scene(int width, int height);
	~Scene();

	void logic(float deltaTime);
	void render();

	static void initSingletons(int width, int height);
	static void destorySingletons();

	static Camera *getCamera();
	static DirectionLight *getLight();

private:
	std::vector<Picture *> pictures;

	static Camera *camera;
	static DirectionLight *light;

	PictureShader *pictureShader;
	ShadowMapShader *shadowMapShader;
	ShadowMapFBO *shadowMapFBO;
};

#endif //_SCENE_H