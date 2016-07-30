#include "picture.h"
#include "scene.h"

const float ShadowMapDepthOffset = -0.08f;

Picture::Picture(std::string fileName)
{
	texture = new Texture(fileName);

	size = 1.0f;
	translation = glm::vec3();
	rotation = glm::vec3();

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, buffers);

	pos[0] = glm::vec3(1.0f, 1.0f, 0.0f);
	pos[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
	pos[2] = glm::vec3(-1.0f, -1.0f, 0.0f);
	pos[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	texCoord[0] = glm::vec2(0.0f, 1.0f);
	texCoord[1] = glm::vec2(1.0f, 1.0f);
	texCoord[2] = glm::vec2(1.0f, 0.0f);
	texCoord[3] = glm::vec2(0.0f, 0.0f);

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

void Picture::setSize(float size)
{
	this->size = size;
}

void Picture::setPosition(glm::vec3 pos)
{
	this->translation = pos;
}

void Picture::setRotation(glm::vec3 rot)
{
	this->rotation = rot;
}

void Picture::addSize(float size)
{
	this->size += size;
}

void Picture::addPosition(glm::vec3 pos)
{
	this->translation += pos;
}

void Picture::addRotation(glm::vec3 rot)
{
	this->rotation += rot;

	for (auto &r : rotation)
	{
		if (r > 360)
		{
			r -= 360;
		}
		else if(r < 0)
		{
			r += 360;
		}
	}
}

void Picture::renderPass(PictureShader *pictureShader)
{
	glBindVertexArray(vao);

	glm::mat4 M = getModelMatrix(translation, rotation, size);
	glm::mat4 VP = Scene::getCamera()->getViewProjMatrix();
	glm::mat4 LightVP = Scene::getLight()->getOrthoViewMatrix();

	texture->bind(GL_TEXTURE0);

	pictureShader->setMVP(VP * M);
	pictureShader->setLightMVP(LightVP * M);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Picture::shadowMapPass(ShadowMapShader *shadowMapShader)
{
	glBindVertexArray(vao);

	glm::mat4 M = getModelMatrix(translation + glm::vec3(0, 0, ShadowMapDepthOffset), rotation, size);
	glm::mat4 LightVP = Scene::getLight()->getOrthoViewMatrix();

	shadowMapShader->setLightMVP(LightVP * M);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

glm::mat4 Picture::getModelMatrix(glm::vec3 t, glm::vec3 r, float s)
{
	glm::mat4 T = glm::translate(t);
	glm::mat4 R = glm::rotate(r.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(r.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(r.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 scale;
	scale.z = 1.0f;

	int imgW = texture->getWidth();
	int imgH = texture->getHeight();
	if (imgW > imgH)
	{
		scale.x = s;
		scale.y = s * imgH / imgW;
	}
	else
	{
		scale.y = s;
		scale.x = s * imgW / imgH;
	}

	glm::mat4 S = glm::scale(scale);

	return T * R * S;
}