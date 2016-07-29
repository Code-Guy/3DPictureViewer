#version 330 

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec2 TexCoord;

out vec2 FragTexCoord;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(Vertex, 1.0);
	FragTexCoord = TexCoord;
}