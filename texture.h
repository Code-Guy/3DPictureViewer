#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);
	~Texture();

	bool load(const std::string& fileName, GLuint minFilter, GLuint magFilter);
	void bind(GLenum textureUnit);

	bool isValid();

	int width;
	int height;

private:
	bool valid;
	GLuint textureObj;
};

#endif //_TEXTURE_H