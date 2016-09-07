#include "picture.h"
#include "scene.h"
#include <QDebug>
#include <QFileInfo>

float Picture::radius;

const float ShadowMapDepthOffset = -0.01f;

Picture::Picture(std::string filePath)
{
	texture = new Texture(filePath);

	this->filePath = QString::fromLocal8Bit(filePath.c_str());
	this->fileName = QString::fromLocal8Bit(parseFileName(filePath).c_str());

	position = glm::vec3();
	angle = 0;
	isVisible = false;
	isBlur = false;

	QFileInfo fileInfo(this->filePath);
	path = fileInfo.path();
	fileBaseName = fileInfo.baseName();
	fileSuffix = fileInfo.suffix();

	if (fileSuffix == "png" || fileSuffix == "PNG")
	{
		isTransparent = true;
	}
	else
	{
		isTransparent = false;
	}

	setSize(1.0f);

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

	memset(buffers, 0, sizeof(GLuint)* 3);
	vao = 0;
}

Picture::~Picture()
{
	if (buffers[0] != 0)
	{
		glDeleteBuffers(3, buffers);
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
	}
	
	delete texture;
}

bool Picture::isValid()
{
	return texture->isValid();
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

void Picture::setAngle(float angle)
{
	this->angle = angle;
}

int Picture::getWidth()
{
	return texture->getWidth();
}

int Picture::getHeight()
{
	return texture->getHeight();
}

int Picture::getRealWidth()
{
	return texture->getRealWidth();
}

int Picture::getRealHeight()
{
	return texture->getRealHeight();
}

QString Picture::getFileName()
{
	return fileName;
}

QString Picture::getFilePath()
{
	return filePath;
}

QString Picture::getPath()
{
	return path;
}

QString Picture::getFileBaseName()
{
	return fileBaseName;
}

QString Picture::getFileSuffix()
{
	return fileSuffix;
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
		if (isTransparent)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		
		glBindVertexArray(vao);

		glm::mat4 M = getModelMatrix();
		glm::mat4 VP = Scene::getCamera()->getViewProjMatrix();
		glm::mat4 LightVP = Scene::getLight()->getProjViewMatrix();

		texture->bind(GL_TEXTURE0);

		pictureShader->setMVP(VP * M);
		pictureShader->setLightMVP(LightVP * M);
		pictureShader->setBlur(isBlur);
		pictureShader->setAlpha(alpha);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		if (isTransparent)
		{
			glDisable(GL_BLEND);
		}
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

void Picture::setAlpha(float alpha)
{
	this->alpha = alpha;
}

bool Picture::glStuff()
{
	if (buffers[0] == 0)
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(3, buffers);

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

	return texture->attach();
}

bool Picture::hit(glm::vec3 p, glm::vec3 d)
{
	glm::mat4 M = getModelMatrix();
	glm::vec4 mpos4[4];
	glm::vec3 mpos[4];
	for (int i = 0; i < 4; i++)
	{
		mpos4[i] = M * glm::vec4(pos[i], 1.0f);
		mpos[i] = glm::vec3(mpos4[i]) / mpos4[i].w;
	}

	float t = (mpos[0].z - p.z) / d.z;
	if (t > 0)
	{
		float x = p.x + d.x * t;
		float y = p.y + d.y * t;
		if (x > mpos[1].x && x < mpos[0].x && y > mpos[2].y && y < mpos[0].y)
		{
			return true;
		}
	}

	return false;
}

glm::mat4 Picture::getModelMatrix()
{
	glm::mat4 T = glm::translate(position + glm::vec3(-radius * sin(glm::radians(angle)), 0, radius * (1 - cos(glm::radians(angle)))));
	glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0, 1, 0));
	glm::mat4 S = glm::scale(scalar);

	return T * R * S;
}

std::string Picture::parseFileName(std::string filePath)
{
	int pos = filePath.find_last_of('/');
	return std::string(filePath.substr(pos + 1));
}