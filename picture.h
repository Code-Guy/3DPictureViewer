#ifndef _PICTURE_H
#define _PICTURE_H

#include "texture.h"
#include "pictureshader.h"
#include "shadowmapshader.h"

#include <glm/glm.hpp>
   
#define POS_VB 0
#define TEXCOORD_VB 1
#define INDEX_VB 2 

#define POS_LOCATION 0
#define TEXCOORD_LOCATION 1

class Picture
{
public:
	Picture(std::string fileName);
	~Picture();

	void setPosition(glm::vec3 position);
	void setSize(float size);

	float getAngle();
	int getWidth();
	int getHeight();
	unsigned char *getBits();
	void setBits(unsigned char *bits, int w, int h);

	void addAngle(float angle);

	void renderPass(PictureShader *pictureShader);
	void shadowMapPass(ShadowMapShader *shadowMapShader);

	void setVisible(bool isVisible);
	void setBlur(bool isBlur);

	static void setRadius(float radius);

private:
	Texture *texture;

	GLuint vao;
	GLuint buffers[3];

	glm::vec3 pos[4];
	glm::vec2 texCoord[4];
	int indices[6];

	glm::vec3 position;
	glm::vec3 scalar;
	float angle;

	bool isVisible;
	bool isBlur;

	static float radius;

	glm::mat4 getModelMatrix();
};

#endif //_PICTURE_H