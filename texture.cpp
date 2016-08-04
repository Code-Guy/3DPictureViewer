#include "texture.h"
#include <FreeImage/FreeImage.h>
#include <iostream>
#include <tool.h>

using namespace std;

const int MaxSize = 800;
const int GaussianBlurRadius = 5;

Texture::Texture(const std::string& fileName, GLuint minFilter, GLuint magFilter)
{
	textureObj = 0;
	width = 0;
	height = 0;

	srcBits = NULL;
	blankBits = NULL;

	valid = load(fileName, minFilter, magFilter);
}

Texture::~Texture()
{
	if (textureObj != 0)
	{
		glDeleteTextures(1, &textureObj);
	}
	delete srcBits;
	if (blankBits)
	{
		delete blankBits;
	}
}

bool Texture::load(const string& fileName, GLuint minFilter, GLuint magFilter)
{

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(fileName.c_str(), 0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, fileName.c_str());

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	int size = w > h ? w : h;
	if (size > MaxSize)
	{
		w = (float)w * MaxSize / size;
		h = (float)h * MaxSize / size;
	}

	this->width = w;
	this->height = h;

	FIBITMAP* temp = imagen;
	imagen = FreeImage_Rescale(imagen, w, h, FILTER_BILINEAR);
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	srcBits = new unsigned char[4 * w * h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for (int j = 0; j < w*h; j++)
	{
		srcBits[j * 4 + 0] = pixeles[j * 4 + 2];
		srcBits[j * 4 + 1] = pixeles[j * 4 + 1];
		srcBits[j * 4 + 2] = pixeles[j * 4 + 0];
		srcBits[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	//Now generate the OpenGL texture object 
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)srcBits);
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

unsigned char *Texture::getBits()
{
	return srcBits;
}

void Texture::setBits(unsigned char *bits, int w, int h)
{
	if (blankBits == NULL)
	{
		blankBits = new unsigned char[width * height * 4];
		memset(blankBits, 255, width * height * 4);
	}
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, blankBits);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (width - w) / 2, (height - h) / 2, w, h, GL_RGBA, GL_UNSIGNED_BYTE, bits);
}