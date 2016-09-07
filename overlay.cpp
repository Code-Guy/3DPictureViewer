#include "overlay.h"

glm::vec2 Overlay::ScreenSize;

Overlay::Overlay(std::string texturePath, glm::ivec2 coord)
{
	texture = new Texture(texturePath);
	texture->attach();

	pos[0] = glm::vec3(coord.x + texture->getWidth(), coord.y + texture->getHeight(), 0.0f);
	pos[1] = glm::vec3(coord.x, coord.y + texture->getHeight(), 0.0f);
	pos[2] = glm::vec3(coord.x, coord.y, 0.0f);
	pos[3] = glm::vec3(coord.x + texture->getWidth(), coord.y, 0.0f);

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

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, buffers);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Overlay::~Overlay()
{
	if (buffers[0] != 0)
	{
		glDeleteBuffers(3, buffers);
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
	}
}

void Overlay::setEnhance(float enhance)
{
	this->enhance = enhance;
}

void Overlay::render(OverlayShader *overlayShader)
{
	glBindVertexArray(vao);

	texture->bind(GL_TEXTURE0);
	overlayShader->setEnhance(enhance);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Overlay::setScreenSize(glm::vec2 ScreenSize)
{
	Overlay::ScreenSize = ScreenSize;
}