#ifndef _BILLBOARDSHADER_H
#define _BILLBOARDSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class BillboardShader : public Shader
{
public:
	BillboardShader();

	virtual bool init();
	void setMV(glm::mat4 MV);
	void setP(glm::mat4 P);
	void setTextureUnit(unsigned int textureUnit);
	
private:
	GLuint MVLocation;
	GLuint PLocation;
	GLuint textureLocation;
};

#endif //_BILLBOARDSHADER_H