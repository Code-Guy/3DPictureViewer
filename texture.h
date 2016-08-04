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

	bool load(const std::string &fileName, GLuint minFilter, GLuint magFilter);

	void bind(GLenum textureUnit);

	bool isValid();

	int getWidth();
	int getHeight();

	unsigned char *getBits();
	void setBits(unsigned char *bits, int w, int h);

private:
	bool valid;
	GLuint textureObj;

	unsigned char *srcBits;
	unsigned char *blankBits;

	int width;
	int height;
};

#endif //_TEXTURE_H