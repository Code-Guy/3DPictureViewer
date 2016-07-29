#ifndef _PICTURESHADER_H
#define _PICTURESHADER_H

#include "shader.h"

#include <glm/glm.hpp>

class PictureShader : public Shader
{
public:
	PictureShader();

	virtual bool init();

	void setMVP(glm::mat4 MVP);
	void setTextureMap(unsigned int TextureMap);

private:
	GLuint MVPLocation;
	GLuint TextureMapLocation;
};

#endif //_PICTURESHADER_H