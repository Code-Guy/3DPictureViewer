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

	void renderPass(PictureShader *pictureShader);
	void shadowMapPass(ShadowMapShader *shadowMapShader);

	void setPosition(glm::vec3 position);
	void setSize(float size);
	void setBits(unsigned char *bits, int w, int h);

	float getAngle();
	void setAngle(float angle);
	void addAngle(float angle);

	int getWidth();
	int getHeight();
	int getRealWidth();
	int getRealHeight();
	QString getFileName();
	unsigned char *getBits();

	void setVisible(bool isVisible);
	void setBlur(bool isBlur);
	void setAlpha(float alpha);

	bool glStuff();

	static void setRadius(float radius);

private:
	Texture *texture;
	QString fileName;

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
	float alpha;

	static float radius;

	glm::mat4 getModelMatrix();
	std::string getFileName(std::string filePath);
};

#endif //_PICTURE_H