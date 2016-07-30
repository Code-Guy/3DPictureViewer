#ifndef _SHADER_H
#define _SHADER_H

#include <list>
#include <string>
#include <GL/glew.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

typedef std::list<GLuint> ShaderObjList;

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual bool init();
	void enable();

	bool validate();

protected:
	bool addShader(GLenum ShaderType, const char* pFilename);
	bool finalize(bool isValidate = true);

	GLint getUniformLocation(const char* pUniformName);
	GLint getProgramParam(GLint param);

	GLuint shaderProg;    

private:
	ShaderObjList shaderObjList;

	static bool readFile(const char *pFileName, std::string &outFile);
};

#endif //_SHADER_H