#ifndef _SHADOWMAPSHADER_H
#define _SHADOWMAPSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader();

	virtual bool init();

	void setLightMVP(glm::mat4 LightMVP);

private:
	GLuint LightMVPLocation;
};

#endif //_SHADOWMAPSHADER_H