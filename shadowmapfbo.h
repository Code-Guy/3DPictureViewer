#ifndef _SHADOWMAPFBO_H
#define _SHADOWMAPFBO_H

#include <GL/glew.h>

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool init(int width, int height);
	void bindForWriting();
	void bindForReading(GLenum textureUnit);

	void unBind();

private:
	GLuint fbo;
	GLuint shadowMap;
};

#endif //_SHADOWMAPFBO_H