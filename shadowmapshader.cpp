#include "shadowmapshader.h"

#include <glm/gtc/type_ptr.hpp>

ShadowMapShader::ShadowMapShader()
{

}

bool ShadowMapShader::init()
{
	if (!Shader::init())
	{
		return false;
	}
	if (!addShader(GL_VERTEX_SHADER, "Resources/shaders/shadowmapping.vert")) 
	{
		return false;
	}
	if (!finalize()) {
		return false;
	}

	LightMVPLocation = getUniformLocation("LightMVP");
	if (LightMVPLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void ShadowMapShader::setLightMVP(glm::mat4 LightMVP)
{
	glUniformMatrix4fv(LightMVPLocation, 1, false, glm::value_ptr(LightMVP));
}