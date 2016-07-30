#version 330 

layout (location = 0) in vec3 Postion;
layout (location = 1) in vec2 TexCoord;

out vec2 FragTexCoord;
out vec4 LightSpacePosition;

uniform mat4 MVP;
uniform mat4 LightMVP;

void main()
{
	gl_Position = MVP * vec4(Postion, 1.0);
	FragTexCoord = TexCoord;
	LightSpacePosition = LightMVP * vec4(Postion, 1.0);
}