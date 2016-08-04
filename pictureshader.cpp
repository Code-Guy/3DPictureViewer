#include "pictureshader.h"

#include <glm/gtc/type_ptr.hpp>

PictureShader::PictureShader()
{

}

bool PictureShader::init()
{
	if (!Shader::init()) 
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/shaders/picture.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/shaders/picture.frag"))
	{
		return false;
	}
	if (!finalize()) 
	{
		return false;
	}

	MVPLocation = getUniformLocation("MVP");
	if (MVPLocation == INVALID_UNIFORM_LOCATION) 
	{
		return false;
	}

	LightMVPLocation = getUniformLocation("LightMVP");
	if (LightMVPLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	TextureMapLocation = getUniformLocation("TextureMap");
	if (TextureMapLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	ShadowMapLocation = getUniformLocation("ShadowMap");
	if (ShadowMapLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	IsBlurLocation = getUniformLocation("IsBlur");
	if (IsBlurLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void PictureShader::setMVP(glm::mat4 MVP)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(MVP));
}

void PictureShader::setLightMVP(glm::mat4 LightMVP)
{
	glUniformMatrix4fv(LightMVPLocation, 1, false, glm::value_ptr(LightMVP));
}

void PictureShader::setTextureMap(unsigned int TextureMap)
{
	glUniform1i(TextureMapLocation, TextureMap);
}

void PictureShader::setShadowMap(unsigned int ShadowMap)
{
	glUniform1i(ShadowMapLocation, ShadowMap);
}

void PictureShader::setBlur(bool flag)
{
	glUniform1i(IsBlurLocation, flag);
}