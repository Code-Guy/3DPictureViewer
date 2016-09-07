#ifndef _OVERLAY_H
#define _OVERLAY_H

#include "texture.h"
#include "overlayshader.h"

class Overlay
{
public:
	Overlay(std::string texturePath, glm::ivec2 coord);
	~Overlay();

	void setEnhance(float enhance);
	void render(OverlayShader *overlayShader);

	static void setScreenSize(glm::vec2 ScreenSize);

private:
	Texture *texture;
	float enhance;

	GLuint vao;
	GLuint buffers[3];

	glm::vec3 pos[4];
	glm::vec2 texCoord[4];
	int indices[6];

	static glm::vec2 ScreenSize;
};

#endif //_OVERLAY_H