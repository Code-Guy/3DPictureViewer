#include "billboardshader.h"
#include <glm/gtc/type_ptr.hpp>

BillboardShader::BillboardShader()
{

}

bool BillboardShader::init()
{
	if (!Shader::init()) {
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/shaders/billboard.vert")) {
		return false;
	}
	if (!addShader(GL_GEOMETRY_SHADER, "Resources/shaders/billboard.geo")) {
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/shaders/billboard.frag")) {
		return false;
	}
	if (!finalize()) {
		return false;
	}

	MVLocation = getUniformLocation("MV");
	if (MVLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	PLocation = getUniformLocation("P");
	if (PLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	textureLocation = getUniformLocation("TextureMap");
	if (textureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void BillboardShader::setMV(glm::mat4 MV)
{
	glUniformMatrix4fv(MVLocation, 1, false, glm::value_ptr(MV));
}

void BillboardShader::setP(glm::mat4 P)
{
	glUniformMatrix4fv(PLocation, 1, false, glm::value_ptr(P));
}

void BillboardShader::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(textureLocation, textureUnit);
}