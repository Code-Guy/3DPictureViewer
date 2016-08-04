#include "picture.h"
#include "scene.h"

float Picture::radius;

const float ShadowMapDepthOffset = -0.01f;

Picture::Picture(std::string fileName)
{
	texture = new Texture(fileName);

	position = glm::vec3();
	angle = 0;
	isVisible = false;
	isBlur = false;

	setSize(1.0f);

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, buffers);

	pos[0] = glm::vec3(1.0f, 1.0f, 0.0f);
	pos[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
	pos[2] = glm::vec3(-1.0f, -1.0f, 0.0f);
	pos[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	texCoord[0] = glm::vec2(1.0f, 1.0f);
	texCoord[1] = glm::vec2(0.0f, 1.0f);
	texCoord[2] = glm::vec2(0.0f, 0.0f);
	texCoord[3] = glm::vec2(1.0f, 0.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POS_LOCATION);
	glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXCOORD_LOCATION);
	glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Picture::~Picture()
{
	if (buffers[0] != 0)
	{
		glDeleteBuffers(3, buffers);
	}

	delete texture;
}

void Picture::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Picture::setSize(float size)
{
	scalar.z = 1.0f;

	int imgW = texture->getWidth();
	int imgH = texture->getHeight();
	if (imgW > imgH)
	{
		scalar.x = size;
		scalar.y = size * imgH / imgW;
	}
	else
	{
		scalar.y = size;
		scalar.x = size * imgW / imgH;
	}
}

float Picture::getAngle()
{
	return angle;
}

int Picture::getWidth()
{
	return texture->getWidth();
}

int Picture::getHeight()
{
	return texture->getHeight();
}

unsigned char *Picture::getBits()
{
	return texture->getBits();
}

void Picture::setBits(unsigned char *bits, int w, int h)
{
	texture->setBits(bits, w, h);
}

void Picture::setRadius(float radius)
{
	Picture::radius = radius;
}

void Picture::addAngle(float angle)
{
	this->angle += angle;
}

void Picture::renderPass(PictureShader *pictureShader)
{
	if (isVisible)
	{
		glBindVertexArray(vao);

		glm::mat4 M = getModelMatrix();
		glm::mat4 VP = Scene::getCamera()->getViewProjMatrix();
		glm::mat4 LightVP = Scene::getLight()->getProjViewMatrix();

		texture->bind(GL_TEXTURE0);

		pictureShader->setMVP(VP * M);
		pictureShader->setLightMVP(LightVP * M);
		pictureShader->setBlur(isBlur);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}

void Picture::shadowMapPass(ShadowMapShader *shadowMapShader)
{
	if (isVisible)
	{
		glBindVertexArray(vao);

		glm::mat4 M = glm::translate(glm::vec3(0, 0, ShadowMapDepthOffset)) * getModelMatrix();
		glm::mat4 LightVP = Scene::getLight()->getProjViewMatrix();

		shadowMapShader->setLightMVP(LightVP * M);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}

void Picture::setVisible(bool isVisible)
{
	this->isVisible = isVisible;
}

void Picture::setBlur(bool isBlur)
{
	this->isBlur = isBlur;
}

glm::mat4 Picture::getModelMatrix()
{
	glm::mat4 T = glm::translate(position + glm::vec3(-radius * sin(glm::radians(angle)), 0, radius * (1 - cos(glm::radians(angle)))));
	glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0, 1, 0));
	glm::mat4 S = glm::scale(scalar);

	return T * R * S;
}