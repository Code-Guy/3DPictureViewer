#version 330

in vec2 FragTexCoord;

out vec4 FragColor;

uniform sampler2D TextureMap;

void main()
{
	FragColor = texture(TextureMap, FragTexCoord);
}