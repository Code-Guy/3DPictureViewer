#ifndef _PICTURESHADER_H
#define _PICTURESHADER_H

#include "shader.h"

#include <glm/glm.hpp>

class PictureShader : public Shader
{
public:
	PictureShader();

	virtual bool init();

	void setMVP(glm::mat4 MVP);
	void setLightMVP(glm::mat4 LightMVP);

	void setTextureMap(unsigned int TextureMap);
	void setShadowMap(unsigned int ShadowMap);
	void setBlur(bool flag);
	void setAlpha(float alpha);

private:
	GLuint MVPLocation;
	GLuint LightMVPLocation;

	GLuint TextureMapLocation;
	GLuint ShadowMapLocation;

	GLuint IsBlurLocation;
	GLuint AlphaLocation;
};

#endif //_PICTURESHADER_H