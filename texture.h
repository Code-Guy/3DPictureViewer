#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <QImage>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string &fileName, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);
	~Texture();

	void bind(GLenum textureUnit);

	int getWidth();
	int getHeight();

	int getRealWidth();
	int getRealHeight();

	unsigned char *getBits();
	void setBits(unsigned char *bits, int w, int h);

	bool attach();

private:
	bool compress;

	GLuint minFilter;
	GLuint magFilter;

	GLuint textureObj;

	unsigned char *srcBits;
	unsigned char *blankBits;

	int width;
	int height;

	int realWidth;
	int realHeight;

	void load(const std::string &fileName);
};

#endif //_TEXTURE_H