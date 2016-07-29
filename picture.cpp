#include "picture.h"
#include "camera.h"

Picture::Picture(std::string fileName)
{
	texture = new Texture(fileName);

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, buffers);

	pos[0] = glm::vec3(-1.0f, 1.0f, 0.0f);
	pos[1] = glm::vec3(1.0f, 1.0f, 0.0f);
	pos[2] = glm::vec3(1.0f, -1.0f, 0.0f);
	pos[3] = glm::vec3(-1.0f, -1.0f, 0.0f);

	texCoord[0] = glm::vec2(0.0f, 0.0f);
	texCoord[1] = glm::vec2(1.0f, 0.0f);
	texCoord[2] = glm::vec2(1.0f, 1.0f);
	texCoord[3] = glm::vec2(0.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos[0]) * 4, pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POS_LOCATION);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord[0]) * 4, texCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXCOORD_LOCATION);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * 6, indices, GL_STATIC_DRAW);

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

void Picture::render(PictureShader *pictureShader)
{
	glBindVertexArray(vao);

	glm::mat4 M = getModelMatrix();
	glm::mat4 VP = Camera::getCamera()->getViewProjMatrix();

	pictureShader->setMVP(VP * M);

	glBindVertexArray(0);
}

glm::mat4 Picture::getModelMatrix()
{
	glm::mat4 T = glm::translate(translation);
	glm::mat4 R = glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 scale;
	scale.z = 1.0f;

	int imgW = texture->getWidth();
	int imgH = texture->getHeight();
	if (imgW > imgH)
	{
		scale.x = size;
		scale.y = size * imgH / imgW;
	}
	else
	{
		scale.y = size;
		scale.x = size * imgW / imgH;
	}

	glm::mat4 S = glm::scale(scale);

	return T * R * S;
}