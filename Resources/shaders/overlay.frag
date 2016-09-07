#version 330

in vec2 FragTexCoord;

out vec4 FragColor;

uniform sampler2D TextureMap;
uniform float Enhance;

void main()
{
	FragColor = texture(TextureMap, vec2(FragTexCoord.x, FragTexCoord.y));
	FragColor.rgb = clamp(FragColor.rgb * Enhance, 0, 1);
}