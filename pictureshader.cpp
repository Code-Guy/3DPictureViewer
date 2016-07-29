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
	if (MVPLocation == 0xffffffff) 
	{
		return false;
	}

	TextureMapLocation = getUniformLocation("TextureMap");
	if (TextureMapLocation == 0xffffffff)
	{
		return false;
	}

	return true;
}

void PictureShader::setMVP(glm::mat4 MVP)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(MVP));
}

void PictureShader::setTextureMap(unsigned int TextureMap)
{
	glUniform1i(TextureMapLocation, TextureMap);
}