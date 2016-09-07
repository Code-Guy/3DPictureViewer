#include "overlayshader.h"

#include <glm/gtc/type_ptr.hpp>

OverlayShader::OverlayShader()
{

}

bool OverlayShader::init()
{
	if (!Shader::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/shaders/overlay.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/shaders/overlay.frag"))
	{
		return false;
	}
	if (!finalize())
	{
		return false;
	}

	ScreenSizeLocation = getUniformLocation("ScreenSize");
	if (ScreenSizeLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	TextureMapLocation = getUniformLocation("TextureMap");
	if (TextureMapLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	EnhanceLocation = getUniformLocation("Enhance");
	if (EnhanceLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void OverlayShader::setScreenSize(glm::vec2 screenSize)
{
	glUniform2fv(ScreenSizeLocation, 1, glm::value_ptr(screenSize));
}

void OverlayShader::setTextureMap(unsigned int TextureMap)
{
	glUniform1i(TextureMapLocation, TextureMap);
}

void OverlayShader::setEnhance(float enhance)
{
	glUniform1f(EnhanceLocation, enhance);
}