#include "texture.h"
#include <iostream>
#include <tool.h>

using namespace std;

const int MaxSize = 1024;
const int GaussianBlurRadius = 5;

Texture::Texture(const std::string& fileName, GLuint minFilter, GLuint magFilter) : minFilter(minFilter), magFilter(magFilter)
{
	textureObj = 0;
	width = 0;
	height = 0;

	srcBits = NULL;
	blankBits = NULL;

	valid = load(fileName);
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

bool Texture::load(const string& fileName)
{
	QImage image(QString::fromLocal8Bit(fileName.c_str()));
	if (image.isNull())
	{
		return false;
	}

	int w = image.width();
	int h = image.height();

	realWidth = w;
	realHeight = h;

	int size = w > h ? w : h;
	if (size > MaxSize)
	{
		w = (float)w * MaxSize / size;
		h = (float)h * MaxSize / size;
	}

	width = w;
	height = h;

	image = image.scaled(w, h);

	srcBits = new unsigned char[w * h * 4];
	for (int i = 0; i < h; i++)
	{
		QRgb *rowData = (QRgb *)image.scanLine(i);
		for (int j = 0; j < w; j++)
		{
			QRgb pixelData = rowData[j];
			srcBits[((h - i - 1) * w + j) * 4] = qRed(pixelData);
			srcBits[((h - i - 1) * w + j) * 4 + 1] = qGreen(pixelData);
			srcBits[((h - i - 1) * w + j) * 4 + 2] = qBlue(pixelData);
			srcBits[((h - i - 1) * w + j) * 4 + 3] = qAlpha(pixelData);
		}
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

int Texture::getRealWidth()
{
	return realWidth;
}

int Texture::getRealHeight()
{
	return realHeight;
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

bool Texture::attach()
{
	if (textureObj == 0)
	{
		//Now generate the OpenGL texture object 
		glGenTextures(1, &textureObj);
		glBindTexture(GL_TEXTURE_2D, textureObj);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcBits);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		GLenum huboError = glGetError();
		if (huboError)
		{
			cout << "loading the texture failed: " << huboError << endl;
			return false;
		}
	}
	
	return true;
}