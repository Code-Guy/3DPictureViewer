#include "texture.h"
#include <FreeImage/FreeImage.h>
#include <iostream>

using namespace std;

Texture::Texture(const std::string& fileName, GLuint minFilter, GLuint magFilter)
{
	textureObj = 0;
	width = 0;
	height = 0;

	valid = load(fileName, minFilter, magFilter);
}

Texture::~Texture()
{
	if (textureObj != 0)
	{
		glDeleteTextures(1, &textureObj);
	}
}

bool Texture::load(const string& fileName, GLuint minFilter, GLuint magFilter)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(fileName.c_str(), 0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, fileName.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	this->width = w;
	this->height = h;

	GLubyte* textura = new GLubyte[4 * w * h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for (int j = 0; j < w*h; j++)
	{
		textura[j * 4 + 0] = pixeles[j * 4 + 2];
		textura[j * 4 + 1] = pixeles[j * 4 + 1];
		textura[j * 4 + 2] = pixeles[j * 4 + 0];
		textura[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	//Now generate the OpenGL texture object 
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	GLenum huboError = glGetError();
	if (huboError)
	{
		cout << "loading the texture failed" << endl;
		return false;
	}
	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureObj);
}

bool Texture::isValid()
{
	return valid;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}