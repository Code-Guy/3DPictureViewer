#version 330

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 TexCoord;

uniform vec2 ScreenSize;

out vec2 FragTexCoord;

void main()
{
	vec2 homoPosition = (Position - ScreenSize / 2) / (ScreenSize / 2);

	gl_Position =  vec4(homoPosition, 0, 1);
	FragTexCoord = TexCoord;
}