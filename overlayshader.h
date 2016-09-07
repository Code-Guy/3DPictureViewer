#ifndef _OVERLAYSHADER_H
#define _OVERLAYSHADER_H

#include "shader.h"

#include <glm/glm.hpp>

class OverlayShader : public Shader
{
public:
	OverlayShader();

	virtual bool init();

	void setScreenSize(glm::vec2 screenSize);
	void setTextureMap(unsigned int TextureMap);
	void setEnhance(float enhance);

private:
	GLuint ScreenSizeLocation;
	GLuint TextureMapLocation;
	GLuint EnhanceLocation;
};

#endif //_OVERLAYSHADER_H