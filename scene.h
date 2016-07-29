#ifndef _SCENE_H
#define _SCENE_H

#include "picture.h"
#include "pictureshader.h"

#include <vector>

class Scene
{
public:
	Scene(int width, int height);
	~Scene();

	void logic(float deltaTime);
	void render();

private:
	std::vector<Picture *> pictures;
	PictureShader *pictureShader;
};

#endif //_SCENE_H